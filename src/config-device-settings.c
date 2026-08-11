// SPDX-FileCopyrightText: 2022-2025 Geoffrey D. Bennett <g@b4.vu>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtk/gtk.h>

#include "alsa.h"
#include "config-helpers.h"
#include "widget-drop-down.h"
#include "window-configuration.h"
#include "config-device-settings.h"

// Add a labelled S/PDIF Source dropdown for one direction. sub_label is NULL
// when there is only one control (the input), matching the original single-
// dropdown layout; it names the direction ("Input"/"Output") when both are
// shown so the two dropdowns are distinguishable.
static void add_spdif_source_dropdown(
  GtkWidget *content, struct alsa_elem *elem, const char *sub_label
) {
  if (sub_label) {
    GtkWidget *label = gtk_label_new(sub_label);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(content), label);
  }

  GtkWidget *dropdown = make_drop_down_alsa_elem(elem, NULL);
  gtk_widget_set_halign(dropdown, GTK_ALIGN_START);
  gtk_box_append(GTK_BOX(content), dropdown);
}

void add_device_settings_tab(GtkWidget *notebook, struct alsa_card *card) {
  // "S/PDIF Source Capture Enum" is the input-connector select (scarlett2's
  // name, present on the USB siblings too); "S/PDIF Source Playback Enum" is
  // the output-connector select, exposed independently by the Clarett
  // Thunderbolt fcp-server maps. Show whichever exist.
  struct alsa_elem *spdif_in = get_elem_by_name(
    card->elems, "S/PDIF Source Capture Enum"
  );
  struct alsa_elem *spdif_out = get_elem_by_name(
    card->elems, "S/PDIF Source Playback Enum"
  );

  if (!spdif_in && !spdif_out)
    return;

  GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_widget_set_margin_top(content, 20);
  gtk_widget_set_margin_start(content, 20);
  gtk_widget_set_margin_end(content, 20);
  gtk_widget_set_margin_bottom(content, 20);

  // S/PDIF Source control(s)
  gtk_box_append(GTK_BOX(content), config_bold_label("S/PDIF Source"));

  // Label each direction only when both are present.
  int both = spdif_in && spdif_out;

  if (spdif_in)
    add_spdif_source_dropdown(content, spdif_in, both ? "Input" : NULL);
  if (spdif_out)
    add_spdif_source_dropdown(content, spdif_out, both ? "Output" : NULL);

  gtk_box_append(GTK_BOX(content), config_help_label(
    "Select the S/PDIF connector: None to disable, Optical for the\n"
    "optical connector, or RCA for the coaxial connector. Input and\n"
    "output can be set independently."
  ));

  g_object_set_data(G_OBJECT(content), PAGE_ID_KEY, (gpointer)"device-settings");
  gtk_notebook_append_page(
    GTK_NOTEBOOK(notebook), content, gtk_label_new("Device Settings")
  );
}
