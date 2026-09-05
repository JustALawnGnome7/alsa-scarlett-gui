// SPDX-FileCopyrightText: 2022-2025 Geoffrey D. Bennett <g@b4.vu>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// maximum number of mix outputs
// (Scarlett 4th Gen 18i20 has 12; the Thunderbolt Clarett line has a
// 30x16 mixer, Mix A-P, on every model; the Thunderbolt Red 8Line has a
// 32x32 one, which is the first device to need more than 26)
#define MAX_MIX_OUT 32

// maximum number of mux inputs (4th Gen 18i20 has 53)
#define MAX_MUX_IN 53
