#pragma once

#include "gapi.h"

int gm_key_pressed(char t, char k) { return gapi_key_pressed(t, k); }

char gm_lower_case(char k) {
  int i = (int)k;
  return (i >= (int)'A' && i <= (int)'Z') ? (char)(i + (int)'a' - (int)'A') : k;
}
char gm_upper_case(char k) {
  int i = (int)k;
  return (i >= (int)'a' && i <= (int)'z') ? (char)(i + (int)'A' - (int)'a') : k;
}

void gm_decode_key_shortcut(char key, char *t, char *k) {
  switch (key) {
  case '\0':
    *t = ' ';
    *k = ' ';
    break;
  case 'U':
  case 'D':
  case 'L':
  case 'R':
    *t = 'a';
    *k = gm_lower_case(key);
    break;
  case 'E':
    *t = 's';
    *k = 'x';
    break;
  case 'S':
  case 'C':
  case 'A':
    *t = 'm';
    *k = gm_lower_case(key);
    break;
  default:
    *t = 'c';
    *k = key;
  }
}
char gm_encode_key_shortcut(char t, char k) {
  switch (t) {
  case ' ':
    return '\0';
  case 'a':
  case 'm':
    return gm_upper_case(k);
  case 's':
    switch (k) {
    case 'x':
      return 'E';
    }
  case 'c':
    return k;
  default:
    return '\0';
  }
}

int gm_key(char key) {
  char t, k;
  gm_decode_key_shortcut(key, &t, &k);
  return gm_key_pressed(t, k);
}
