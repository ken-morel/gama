#ifndef GAMA_LOG_H_INCLUDED
#define GAMA_LOG_H_INCLUDED

/**
 * @brief Logs a message to the platform's console.
 * @param txt The text message to log.
 */
void gm_log(const char *txt);

/**
 * @brief Logs a warning message to the platform's console.
 * @param txt The text message to log.
 */
void gm_log_warning(const char *txt);

/**
 * @brief Logs an error message to the platform's console.
 * @param txt The text message to log.
 */
void gm_log_error(const char *txt);

#ifndef GM_NO_GAPI
#include "gapi.h"
void gm_log(const char *txt) { return gapi_log(txt); }
void gm_log_warning(const char *txt) { return gapi_log_warning(txt); }
void gm_log_error(const char *txt) { return gapi_log_error(txt); }
#else
void gm_log(const char *txt) {}
void gm_log_warning(const char *txt) {}
void gm_log_error(const char *txt) {}
#endif

#endif // GAMA_LOG_H_INCLUDED
