#ifndef GAMA_EVENTS_INCLUDED
#define GAMA_EVENTS_INCLUDED

typedef struct {
  double x;
  double y;
  int button;
  int down;
} MouseClickEvent;

#include "_key.h"

typedef struct {
  Key key;

} KeyEvent;

#endif // GAMA_EVENTS_INCLUDED
