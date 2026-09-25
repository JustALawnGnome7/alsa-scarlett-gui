// SPDX-FileCopyrightText: 2022-2025 Geoffrey D. Bennett <g@b4.vu>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

struct alsa_card;

// Get device-specific default port name
// Returns NULL if no device-specific name defined for this port
const char *get_device_port_name(
  struct alsa_card *card, // matched by USB PID, or by card name if not USB
  int port_category, // PC_HW, PC_PCM, PC_MIX
  int hw_type,       // for PC_HW: HW_TYPE_ANALOGUE, HW_TYPE_SPDIF, HW_TYPE_ADAT
  int is_snk,        // 0 = source, 1 = sink
  int port_num       // 0-based port number
);

// Get device-specific default stereo pair name
// Returns NULL if no device-specific name defined for this pair
const char *get_device_pair_name(
  struct alsa_card *card, // matched by USB PID, or by card name if not USB
  int port_category, // PC_HW, PC_PCM, PC_MIX
  int hw_type,       // for PC_HW: HW_TYPE_ANALOGUE, HW_TYPE_SPDIF, HW_TYPE_ADAT
  int is_snk,        // 0 = source, 1 = sink
  int pair_num       // 0-based pair number (pair 0 = ports 1-2, pair 1 = ports 3-4, etc.)
);
