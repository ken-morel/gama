#pragma once

#include "widgets/button.h"
#include "widgets/frame.h"
#include "widgets/joystick.h"
#include "widgets/scale.h"
#include "widgets/switch.h"

#ifndef GAMA_MAX_THEME_CACHE_SIZE
#define GAMA_MAX_THEME_CACHE_SIZE 10
#endif

/**
 * @brief Structure containing all widget themes for caching purposes.
 */
typedef struct {
  gmwButtonTheme button;     /**< Theme for button widgets */
  gmwSwitchTheme switch_;    /**< Theme for switch widgets */
  gmwScaleTheme scale;       /**< Theme for scale widgets */
  gmwJoystickTheme joystick; /**< Theme for joystick widgets */
  gmwFrameTheme frame;       /**< Theme for frame widgets */
} gmwThemeCache;

/**
 * @brief Internal cache array for widget themes.
 */
gmwThemeCache _theme_cache[GAMA_MAX_THEME_CACHE_SIZE];

/**
 * @brief Current index in the theme cache array.
 */
short gm_theme_cache_index = -1;

/**
 * @brief Saves the current widget themes to the theme cache.
 * @return A pointer to the saved theme cache entry, or NULL if the cache is full.
 */
gmwThemeCache *gmw_save() {
  if (gm_theme_cache_index >= GAMA_MAX_THEME_CACHE_SIZE - 1)
    return NULL;
  gm_theme_cache_index++;
  _theme_cache[gm_theme_cache_index].button = gmwButton;
  _theme_cache[gm_theme_cache_index].switch_ = gmwSwitch;
  _theme_cache[gm_theme_cache_index].scale = gmwScale;
  _theme_cache[gm_theme_cache_index].joystick = gmwJoystick;
  _theme_cache[gm_theme_cache_index].frame = gmwFrame;
  return &_theme_cache[gm_theme_cache_index];
}

/**
 * @brief Restores the previous widget themes from the theme cache.
 */
void gmw_restore() {
  if (gm_theme_cache_index < 0)
    return;
  gmwButton = _theme_cache[gm_theme_cache_index].button;
  gmwSwitch = _theme_cache[gm_theme_cache_index].switch_;
  gmwScale = _theme_cache[gm_theme_cache_index].scale;
  gmwJoystick = _theme_cache[gm_theme_cache_index].joystick;
  gmwFrame = _theme_cache[gm_theme_cache_index].frame;
  gm_theme_cache_index--;
}
