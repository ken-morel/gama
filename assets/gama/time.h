
#ifndef GAMA_TIME_INCLUDED
#define GAMA_TIME_INCLUDED
#include <time.h> // Required for the fallback clock() and POSIX clock_gettime()

// Pre-processor directives to select the appropriate timing function
#if defined(_WIN32)
#include <windows.h>

/**
 * @brief Gets the elapsed time since an arbitrary point, in seconds.
 *
 * This version uses the high-resolution Windows Performance Counter for
 * maximum precision.
 *
 * @return The elapsed time in seconds as a double.
 */
double get_elapsed_seconds(void) {
  // A static variable is initialized only once.
  // We get the counter frequency on the first call and store it.
  static LARGE_INTEGER frequency = {0};
  if (frequency.QuadPart == 0) {
    QueryPerformanceFrequency(&frequency);
  }

  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);

  // Divide the current counter value by the frequency to get seconds
  return (double)counter.QuadPart / (double)frequency.QuadPart;
}

#elif defined(__linux__) || defined(__APPLE__)

/**
 * @brief Gets the elapsed time since an arbitrary point, in seconds.
 *
 * This version uses the POSIX clock_gettime() function with CLOCK_MONOTONIC,
 * which provides high precision and is not affected by system time changes.
 *
 * @return The elapsed time in seconds as a double.
 */
double get_elapsed_seconds(void) {
  struct timespec ts;
  // CLOCK_MONOTONIC is a clock that cannot be set and represents monotonic
  // time since some unspecified starting point.
  clock_gettime(CLOCK_MONOTONIC, &ts);
  // Combine seconds and nanoseconds into a single double value
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1.0e9;
}

#else

/**
 * @brief Gets the elapsed time since the program started, in seconds.
 *
 * This is a fallback version using the standard C clock() function.
 * Note: Its precision may be low, and it measures CPU time, not
 * real-world "wall clock" time.
 *
 * @return The elapsed CPU time in seconds as a double.
 */
double get_elapsed_seconds(void) { return (double)clock() / CLOCKS_PER_SEC; }

#endif // End of platform-specific implementations
#endif
