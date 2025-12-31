#pragma once

#include "_malloc.h"

#include <stddef.h>
#include <string.h>
typedef struct {
  size_t length;
  char *content;
} gmStr;

gmStr gm_str() { return (gmStr){.length = 0, .content = NULL}; }

int gm_str_appendn(gmStr *s, size_t n, const char *txt) {
  if (!s || !txt || n == 0)
    return -1;
  if (s->length == 0) {
    s->length = 0;
    s->content = malloc(n + 1);
    s->content[n] = '\0';
  } else {
    size_t newcap = s->length + n + 1;
    char *newtxt = realloc(s->content, newcap);
    newtxt[newcap - 1] = '\0';
    if (!newtxt)
      return -5;
    s->content = newtxt;
  }
  for (size_t i = 0; i < n; i++) {
    s->content[s->length] = txt[i];
    s->length++;
  }
  return 0;
}

static inline int gm_str_append(gmStr *s, const char *txt) {
  return gm_str_appendn(s, strlen(txt), txt);
}

void gm_str_clear(gmStr *str) {
  free(str->content);
  str->length = 0;
}
