// SPDX-FileCopyrightText: 2022-2025 Geoffrey D. Bennett <g@b4.vu>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window-hardware.h"

GtkWidget *window_hardware;

struct hw_info {
  char *name;
};

// a column of a tab: a bold heading over its own list
struct hw_col {
  char *name;
  struct hw_info *info;
};

// a tab: either one list (info), or several side-by-side columns (cols)
struct hw_cat {
  char *name;
  struct hw_info *info;
  struct hw_col *cols;
};

struct hw_info gen_1_info[] = {
  { "Scarlett 6i6" },
  { "Scarlett 8i6" },
  { "Scarlett 18i6" },
  { "Scarlett 18i8" },
  { "Scarlett 18i20" },
  { }
};

struct hw_info gen_2_info[] = {
  { "Scarlett 6i6" },
  { "Scarlett 18i8" },
  { "Scarlett 18i20" },
  { }
};

struct hw_info gen_3_info[] = {
  { "Scarlett Solo" },
  { "Scarlett 2i2" },
  { "Scarlett 4i4" },
  { "Scarlett 8i6" },
  { "Scarlett 18i8" },
  { "Scarlett 18i20" },
  { }
};

struct hw_info gen_4_info[] = {
  { "Scarlett Solo" },
  { "Scarlett 2i2" },
  { "Scarlett 4i4" },
  { "Scarlett 16i16" },
  { "Scarlett 18i16" },
  { "Scarlett 18i20" },
  { }
};

// Thunderbolt, not USB: driven by the out-of-tree snd-clarett driver
// plus fcp-server, not by the in-kernel scarlett2 driver. The original
// Clarett line (2015) has no USB/+ suffix and predates those models, so
// it is listed first among the Claretts.
struct hw_info clarett_thunderbolt_info[] = {
  { "Clarett 2Pre" },
  { "Clarett 4Pre" },
  { "Clarett 8Pre" },
  { "Clarett 8PreX" },
  { }
};

struct hw_info clarett_usb_info[] = {
  { "Clarett 2Pre USB" },
  { "Clarett 4Pre USB" },
  { "Clarett 8Pre USB" },
  { }
};

struct hw_info clarett_plus_info[] = {
  { "Clarett+ 2Pre" },
  { "Clarett+ 4Pre" },
  { "Clarett+ 8Pre" },
  { }
};

// Same driver and the same protocol as the Thunderbolt Claretts, but a
// different product line, so it gets a tab of its own rather than a
// column under Clarett. Its columns split it by Thunderbolt generation.
struct hw_info red_tb2_info[] = {
  { "Red 4Pre (untested)" },
  { "Red 8Pre (untested)" },
  { }
};

struct hw_info red_tb3_info[] = {
  { "Red 8Line" },
  { "Red 16Line (untested)" },
  { }
};

struct hw_info vocaster_info[] = {
  { "Vocaster One" },
  { "Vocaster Two" },
  { }
};

struct hw_col scarlett_cols[] = {
  { "1st Gen", gen_1_info },
  { "2nd Gen", gen_2_info },
  { "3rd Gen", gen_3_info },
  { "4th Gen", gen_4_info },
  { }
};

struct hw_col clarett_cols[] = {
  { "Clarett (TB2)", clarett_thunderbolt_info },
  { "Clarett USB",   clarett_usb_info },
  { "Clarett+",      clarett_plus_info },
  { }
};

struct hw_col red_cols[] = {
  { "Red (TB2)", red_tb2_info },
  { "Red (TB3)", red_tb3_info },
  { }
};

struct hw_cat hw_cat[] = {
  { "Scarlett",
    NULL,
    scarlett_cols
  },
  { "Clarett",
    NULL,
    clarett_cols
  },
  { "Red",
    NULL,
    red_cols
  },
  { "Vocaster",
    vocaster_info
  },
  { }
};

gboolean window_hardware_close_request(
  GtkWindow *w,
  gpointer   data
) {
  GtkApplication *app = data;

  g_action_group_activate_action(
    G_ACTION_GROUP(app), "hardware", NULL
  );
  return true;
}

static gboolean on_key_press(
  GtkEventControllerKey *controller,
  guint                  keyval,
  guint                  keycode,
  GdkModifierType        state,
  gpointer               user_data
) {
  GtkApplication *app = user_data;

  if (keyval == GDK_KEY_Escape) {
    g_action_group_activate_action(G_ACTION_GROUP(app), "hardware", NULL);
    return 1;
  }

  if (state & GDK_CONTROL_MASK) {
    const char *action = NULL;

    switch (keyval) {
      case GDK_KEY_q: action = "quit";     break;
      case GDK_KEY_w:
      case GDK_KEY_h: action = "hardware"; break;
    }

    if (action) {
      g_action_group_activate_action(G_ACTION_GROUP(app), action, NULL);
      return 1;
    }
  }

  return 0;
}

static GtkWidget *make_column(struct hw_col *col) {
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  GtkWidget *heading = gtk_label_new(NULL);
  char *markup = g_markup_printf_escaped("<b><u>%s</u></b>", col->name);

  gtk_label_set_markup(GTK_LABEL(heading), markup);
  g_free(markup);
  gtk_widget_set_halign(heading, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(box), heading);

  for (struct hw_info *info = col->info; info->name; info++) {
    GtkWidget *label = gtk_label_new(info->name);

    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), label);
  }
  return box;
}

GtkWidget *make_notebook_page(struct hw_cat *cat) {
  if (cat->cols) {
    GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);

    // centre the columns in the tab
    gtk_widget_set_halign(row, GTK_ALIGN_CENTER);

    for (struct hw_col *col = cat->cols; col->name; col++) {
      GtkWidget *column = make_column(col);

      gtk_widget_set_valign(column, GTK_ALIGN_START);
      gtk_box_append(GTK_BOX(row), column);
    }
    return row;
  }

  // a single list: one centred column of left-aligned rows, like the
  // columns above but without a heading
  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  for (struct hw_info *info = cat->info; info->name; info++) {
    GtkWidget *label = gtk_label_new(info->name);

    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), label);
  }
  return box;
}

void add_notebook_pages(GtkWidget *notebook) {
  for (struct hw_cat *cat = hw_cat; cat->name; cat++) {
    GtkWidget *page = make_notebook_page(cat);
    GtkWidget *label = gtk_label_new(cat->name);

    // space between each tab's contents and the notebook's border; the
    // window opens at its natural size, so this is also what sets how
    // roomy it opens
    gtk_widget_set_margin_start(page, 20);
    gtk_widget_set_margin_end(page, 20);
    gtk_widget_set_margin_top(page, 15);
    gtk_widget_set_margin_bottom(page, 15);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page, label);
  }
}

void create_hardware_window(GtkApplication *app) {
  window_hardware = gtk_window_new();
  g_signal_connect(
    window_hardware,
    "close_request",
    G_CALLBACK(window_hardware_close_request),
    app
  );

  GtkEventController *key_controller = gtk_event_controller_key_new();
  gtk_widget_add_controller(window_hardware, key_controller);
  g_signal_connect(
    key_controller, "key-pressed", G_CALLBACK(on_key_press), app
  );

  gtk_window_set_title(
    GTK_WINDOW(window_hardware),
    "ALSA Scarlett Supported Hardware"
  );

  GtkWidget *top = gtk_frame_new(NULL);
  gtk_widget_add_css_class(top, "window-frame");
  gtk_window_set_child(GTK_WINDOW(window_hardware), top);

  // No "window-content" class: the notebook's own border is suppressed by
  // the ".window-frame notebook" rule (its pages carry the red border), so
  // all that class added here was its padding, which doubled the space
  // between the window's edge and the red border compared with the main
  // window.
  GtkWidget *notebook = gtk_notebook_new();
  gtk_frame_set_child(GTK_FRAME(top), notebook);

  add_notebook_pages(notebook);
}
