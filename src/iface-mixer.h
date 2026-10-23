// SPDX-FileCopyrightText: 2022-2025 Geoffrey D. Bennett <g@b4.vu>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "alsa.h"

// Structure to store input gain widget and its routing source
struct input_gain_widget {
  GtkWidget          *widget;
  int                 port_num;     // 1-based port number
  struct routing_src *r_src;        // routing source for level lookup
};

// Structure to store output gain widget and its routing sink
struct output_gain_widget {
  GtkWidget          *widget;
  int                 port_num;     // 1-based port number
  struct routing_snk *r_snk;        // routing sink for level lookup
  struct routing_snk *r_snk2;       // second channel of a stereo dial, or NULL
};

GtkWidget *create_iface_mixer_main(struct alsa_card *card);

// The group analogue output port_num (1-based) belongs to on a card that
// names its outputs in groups (the Focusrite Red: "Monitor", "Headphones 1",
// "Line Output"), or NULL. *pos is its 1-based position in the group; *knob
// is set if the group is a stereo pair sharing one knob. g_free the result.
char *get_output_group(
  struct alsa_card *card,
  int               port_num,
  int              *pos,
  int              *knob
);
