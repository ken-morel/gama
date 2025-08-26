
#ifndef GAMA_TIME_INCLUDED
#define GAMA_TIME_INCLUDED
#include <time.h>

#if defined(_WIN32)
#include <windows.h>

double get_elapsed_seconds(void) {
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

double get_elapsed_seconds(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  // Combine seconds and nanoseconds into a single double value
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1.0e9;
}

#else

double get_elapsed_seconds(void) { return (double)clock() / CLOCKS_PER_SEC; }

#endif
#endif
