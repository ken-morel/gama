#pragma once

/**
 * @internal
 * @brief Stores the delta time (time since last frame) in seconds.
 * This value is updated by `gapi_yield`.
 */
double _gm_dt = 0;
/**
 * @internal
 * @brief Stores the total accumulated time since the engine started, in
 * seconds. This value is updated by `gapi_yield`.
 */
double _gm_t = 0;

/**
 * @brief Retrieves the delta time (time since the last frame) in seconds.
 * @return The delta time.
 */
static inline double gm_dt() { return _gm_dt; }
/**
 * @brief Retrieves the total accumulated time since the engine started, in
 * seconds.
 * @return The total elapsed time.
 */
static inline double gm_t() { return _gm_t; }
