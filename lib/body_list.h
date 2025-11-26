#pragma once

#include "body.h"
#include <stdio.h>


typedef gmBody **gmBodies;

size_t gm_bodies_length(gmBodies list) {
  if (list == NULL)
    return 0;
  else
    for (size_t i = 0;; i++)
      if (list[i] == NULL)
        return i;
}
size_t gm_bodies_count(gmBodies list, gmBody* obj) {
  size_t count = 0;
  for (size_t i = 0;; i++) {
    if (list[i] == obj)
      count++;
    else if (list[i] == NULL)
      return count;
  }
}
gmBodies gm_bodies_push(gmBodies list, gmBody* obj) {
  size_t length = gm_bodies_length(list);
  gmBodies new_list = reallocarray(list, length + 2, sizeof(gmBody*));
  new_list[length] = obj;
  new_list[length + 1] = NULL;
  return new_list;
}

gmBodies gm_bodies_pop(gmBodies list) {
  size_t len = gm_bodies_length(list);
  gmBodies new_list = reallocarray(list, len, sizeof(gmBody*));
  new_list[len - 1] = NULL;
  return new_list;
}
gmBodies gm_bodies_remove(gmBodies list, gmBody* obj) {
  size_t len = gm_bodies_length(list);
  size_t count = gm_bodies_count(list, obj);
  gmBodies new_list = malloc((len - count + 1) * sizeof(gmBody*));
  size_t index = 0;
  for (size_t i = 0; i < len; i++) {
    if (list[i] != obj) {
      new_list[index] = list[i];
      index++;
    }
  }
  return new_list;
}
