#pragma once
#include "gapi.h"

int gama_key_down(char t, char k) { return gapi_key_down(t, k); }
int gama_key_pressed(char t, char k) { return gapi_key_pressed(t, k); }

char gama_lower_case(char k) {
  int i = (int)k;
  return (i >= (int)'A' && i <= (int)'Z') ? (char)(i + (int)'a' - (int)'A') : k;
}

void gama_decode_key_shortcut(char key, char *t, char *k) {
  switch (key) {
  case 'U':
  case 'D':
  case 'L':
  case 'R':
    *t = 'a';
    *k = gama_lower_case(key);
    break;
  case '\n':
    *t = 's';
    *k = 'a';
    break;
  case 'E':
    *t = 's';
    *k = 'x';
    break;
  case 'S':
  case 'C':
  case 'A':
    *t = 'm';
    *k = gama_lower_case(key);
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
