// SPDX-FileCopyrightText: 2022-2025 Geoffrey D. Bennett <g@b4.vu>
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

int get_num_from_string(const char *s);
void get_two_num_from_string(const char *s, int *a, int *b);
int string_ends_with(const char *s, const char *suffix);

// Mix output labels.
//
// Mixes are named "Mix A", "Mix B", ... The obvious 'A' + i runs off the
// end of the alphabet on a device with more than 26 of them (the
// Thunderbolt Red 8Line has 32), landing on "Mix [", "Mix \\", "Mix ]" --
// which GKeyFile then rejects as key names, silently losing those mixes
// from the saved device state. So past Z the labels continue AA, AB, ...
// (bijective base 26, the spreadsheet-column scheme), matching what
// fcp-server generates.
//
// mix_num_to_label() writes the label for a 0-based mix number into buf
// and returns it. mix_label_to_num() parses one, returning the 0-based
// mix number and, if end is non-NULL, where the label stopped; it returns
// -1 if s does not start with a letter.
char *mix_num_to_label(int mix_num, char *buf, int len);
int mix_label_to_num(const char *s, const char **end);
