#pragma once
#include <stdio.h>

#define GAMA_STRING_MAX_SIZE 256

typedef struct {
  char text[GAMA_STRING_MAX_SIZE];
} gama_string;

// A safe way to create a string by copying from a C string literal
static inline gama_string gama_string_create(const char* initial_text) {
  gama_string str;
  if (initial_text) {
    snprintf(str.text, GAMA_STRING_MAX_SIZE, "%s", initial_text);
  } else {
    str.text[0] = '\0';
  }
  return str;
}

// A safe way to set a string's value
static inline void gama_string_set(gama_string* str, const char* new_text) {
  if (str && new_text) {
    snprintf(str->text, GAMA_STRING_MAX_SIZE, "%s", new_text);
  } else if (str) {
    str->text[0] = '\0';
  }
}

// A helper to get the raw C-string for passing to other functions
static inline const char* gama_string_get(const gama_string* str) {
  return str ? str->text : "";
}
