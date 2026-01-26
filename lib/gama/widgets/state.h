#ifndef GM_WIDGETS_STATE_H_INCLUDED
#define GM_WIDGETS_STATE_H_INCLUDED

#include <stdint.h>
typedef enum {
  gmwUnknown = 0,
  gmwActive = 3,
  gmwFocussed = 2,
  gmwNormal = 1
} gmwState;

#endif // GM_WIDGETS_STATE_H_INCLUDED
