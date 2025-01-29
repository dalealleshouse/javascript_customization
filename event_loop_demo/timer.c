#include "timer.h"
#include <stdbool.h>
#include <time.h>

struct timespec now_timespec(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts;
}

bool timespec_ge(const struct timespec *a, const struct timespec *b) {
  if (a->tv_sec > b->tv_sec)
    return true;
  if (a->tv_sec < b->tv_sec)
    return false;
  // same seconds, compare nanoseconds
  return (a->tv_nsec >= b->tv_nsec);
}

void timespec_add_ms(struct timespec *ts, int ms) {
  ts->tv_sec += ms / 1000;
  ts->tv_nsec += (ms % 1000) * 1000000L;
  if (ts->tv_nsec >= 1000000000L) {
    ts->tv_sec += 1;
    ts->tv_nsec -= 1000000000L;
  }
}
