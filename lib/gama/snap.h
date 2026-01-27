#ifndef GAMA_SNAP_H_INCLUDED
#define GAMA_SNAP_H_INCLUDED

#include "draw.h"
#include "log.h"
#include "window.h"
#include <stdio.h>
#ifndef GM_MAX_SNAPS
#define GM_MAX_SNAPS 50
#endif

#include "image.h"
#include "position.h"
#ifndef GM_NO_GAPI

void gm_background(gmColor c);

int _gm_snaped[GM_MAX_SNAPS];
int _gm_snapping = 0;
void _gm_order_snaps() {
  size_t shift = 0;
  for (size_t i = 0; i < GM_MAX_SNAPS; i++) {
    if (_gm_snaped[i] == 0)
      shift += 1;
    else {
      _gm_snaped[i - shift] = _gm_snaped[i];
      _gm_snaped[i] = 0;
    }
  }
}
void gm_unsnap(int id) {
  if (!id) {
    memset(_gm_snaped, 0, sizeof(_gm_snaped));
  } else {
    for (size_t i = 0; i < GM_MAX_SNAPS; i++)
      if (_gm_snaped[i] == id)
        _gm_snaped[i] = 0;
    _gm_order_snaps();
  }
}

void gm_snaped_cancel() { _gm_snapping = 0; }

int32_t gm_snap(gmImage *img, int handle) {
  img->handle = handle;
  return gapi_snap(handle, &img->width, &img->height);
}

int gm_snaped(int id) {
  if (id == 0)
    return 0;
  for (size_t i = 0; i < GM_MAX_SNAPS; i++) {
    if (_gm_snaped[i] == id) {
      gapi_draw_image(id, 0, 0, 0, 0);
      return 0;
    }
  }
  if (_gm_snapping == 0 || _gm_snapping != id) {
    // clears previous snap
    _gm_snapping = id; // clear and record next frame
    // gapi_set_background_color(GM_TRANSPARENT);
    gapi_sync();
    gapi_clear();
    gapi_sync();
    gapi_set_background_color(gmWindow.background);
    // gapi_sync();
    return 1;
  } else if (_gm_snapping == id) {
    gmImage snap;
    if (0 == gm_snap(&snap, id)) {
      int found = 0;
      for (size_t i = 0; i < GM_MAX_SNAPS; i++) {
        if (_gm_snaped[i] == 0) {
          _gm_snaped[i] = id;
          found = 1;
          break;
        }
      }
      if (!found) {
        _gm_snaped[0] = 0;
        _gm_order_snaps();
        _gm_snaped[GM_MAX_SNAPS - 1] = id;
      }
    }
    _gm_snapping = 0;
    return 0;
  } else {
    gm_log_error("This should not happen! unexpected gm_snaped branch");
    return -1;
  }
}

gmPos gm_get_size() {
  // size_t
  uint32_t w, h;
  gapi_get_size(&w, &h);
  return gmpos(w, h);
}
#endif

#endif // GAMA_SNAP_H_INCLUDED
