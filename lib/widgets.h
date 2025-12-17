#include "widgets/button.h"
#include "widgets/frame.h"
#include "widgets/joystick.h"
#include "widgets/scale.h"
#include "widgets/switch.h"
#ifndef GAMA_MAX_THEME_CACHE_SIZE
#define GAMA_MAX_THEME_CACHE_SIZE 10
#endif

typedef struct {
  gmwButtonTheme button;
  gmwSwitchTheme switch_;
  gmwScaleTheme scale;
  gmwJoystickTheme joystick;
} gmwThemeCache;
gmwThemeCache _theme_cache[GAMA_MAX_THEME_CACHE_SIZE];
short gm_theme_cache_index = -1;

gmwThemeCache *gmw_save() {
  if (gm_theme_cache_index >= GAMA_MAX_THEME_CACHE_SIZE - 1)
    return NULL;
  gm_theme_cache_index++;
  _theme_cache[gm_theme_cache_index].button = gmwButton;
  _theme_cache[gm_theme_cache_index].switch_ = gmwSwitch;
  _theme_cache[gm_theme_cache_index].scale = gmwScale;
  _theme_cache[gm_theme_cache_index].joystick = gmwJoystick;
  return &_theme_cache[gm_theme_cache_index];
}
gmwThemeCache *gmw_restore() {
  if (gm_theme_cache_index < 0)
    return NULL;
  gmwButton = _theme_cache[gm_theme_cache_index].button;
  gmwSwitch = _theme_cache[gm_theme_cache_index].switch_;
  gmwScale = _theme_cache[gm_theme_cache_index].scale;
  gmwJoystick = _theme_cache[gm_theme_cache_index].joystick;
  gm_theme_cache_index--;
  return &_theme_cache[gm_theme_cache_index + 1];
}
