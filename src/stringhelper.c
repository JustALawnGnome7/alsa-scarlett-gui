// SPDX-FileCopyrightText: 2022-2025 Geoffrey D. Bennett <g@b4.vu>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "stringhelper.h"

// return the first number found in the string
int get_num_from_string(const char *s) {
  int num;

  while (*s) {
    if (isdigit(*s))
      break;
    s++;
  }

  if (!*s)
    return -1;

  if (!sscanf(s, "%d", &num))
    return 0;

  return num;
}

// return the first two numbers found in the string
void get_two_num_from_string(const char *s, int *a, int *b) {
  *a = -1;
  *b = -1;

  while (*s) {
    if (isdigit(*s))
      break;
    s++;
  }

  if (!*s)
    return;

  if (!sscanf(s, "%d", a))
    return;

  while (*s) {
    if (!isdigit(*s))
      break;
    s++;
  }

  while (*s) {
    if (isdigit(*s))
      break;
    s++;
  }

  if (!sscanf(s, "%d", b))
    return;
}

// check if the given string ends with the given suffix
int string_ends_with(const char *s, const char *suffix) {
  if (!s || !suffix)
    return 0;
  int s_len = strlen(s);
  int suffix_len = strlen(suffix);
  if (s_len < suffix_len)
    return 0;
  return strcmp(s + s_len - suffix_len, suffix) == 0;
}

// see stringhelper.h
char *mix_num_to_label(int mix_num, char *buf, int len) {
  char tmp[8];
  int n = 0;

  if (mix_num < 0)
    mix_num = 0;

  // bijective base 26, least-significant letter first
  do {
    tmp[n++] = 'A' + mix_num % 26;
    mix_num = mix_num / 26 - 1;
  } while (mix_num >= 0 && n < (int)sizeof(tmp));

  int i = 0;
  while (n > 0 && i < len - 1)
    buf[i++] = tmp[--n];
  buf[i] = '\0';

  return buf;
}

// see stringhelper.h
int mix_label_to_num(const char *s, const char **end) {
  if (!s || *s < 'A' || *s > 'Z') {
    if (end)
      *end = s;
    return -1;
  }

  int num = 0;

  while (*s >= 'A' && *s <= 'Z') {
    num = num * 26 + (*s - 'A' + 1);
    s++;
  }

  if (end)
    *end = s;

  return num - 1;
}
