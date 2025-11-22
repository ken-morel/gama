#pragma once

#include "body.h"

typedef gama_body *gbody;
typedef gbody *gama_body_list;

gama_body_list gama_bodys = gnothing;

size_t gama_body_list_length(gama_body_list list) {
  for (size_t i = 0;; i++)
    if (list[i] == NULL)
      return i;
}
size_t gama_body_list_count(gama_body_list list, gbody obj) {
  size_t count = 0;
  for (size_t i = 0;; i++) {
    if (list[i] == obj)
      count++;
    else if (list[i] == NULL)
      return count;
  }
}
gama_body_list gama_body_list_add(gama_body_list list, gbody obj) {
  size_t length = gama_body_list_length(list);
  gama_body_list new_list = reallocarray(list, length + 2, sizeof(gbody));
  new_list[length] = obj;
  new_list[length + 1] = NULL;
  return new_list;
}

gama_body_list gama_body_list_pop(gama_body_list list) {
  size_t len = gama_body_list_length(list);
  gama_body_list new_list = reallocarray(list, len + 1, sizeof(gbody));
  new_list[len] = NULL;
  return new_list;
}
gama_body_list gama_body_list_remove(gama_body_list list, gbody obj) {
  size_t len = gama_body_list_length(list);
  size_t count = gama_body_list_count(list, obj);
  gama_body_list new_list = malloc((len - count + 1) * sizeof(gbody));
  size_t index = 0;
  for (size_t i = 0; i < len; i++) {
    if (list[i] != obj) {
      new_list[i] = list[i];
      i++;
    }
  }
  return new_list;
}
