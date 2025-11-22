#pragma once
#include "gapi.h"

int gama_key_down(char t, char k) { return gapi_key_down(t, k); }
int gama_key_pressed(char t, char k) { return gapi_key_pressed(t, k); }

void gama_decode_key_shortcut(char key, char *t, char *k) {
  switch (key) {
  case 'U':
  case 'D':
  case 'L':
  case 'R':
    *t = 'a';
    *k = (char)((int)key + (int)'a' - (int)'A');
    break;
  default:
    *t = 'c';
    *k = key;
  }
}

int gama_key_shortcut_pressed(char key) {
  char t, k;
  gama_decode_key_shortcut(key, &t, &k);
  return gama_key_pressed(t, k);
}

int gama_key_shortcut_down(char key) {
  char t, k;
  gama_decode_key_shortcut(key, &t, &k);
  return gama_key_down(t, k);
}
