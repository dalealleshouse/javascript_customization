#pragma once
#include "../../quickjs/quickjs.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  // When should this timer fire?
  struct timespec deadline;

  // The callback function stored as a JSValue
  JSValue callback;

  // We need the context to call the function
  JSContext *ctx;
} TimerItem;

// ---------- Utility: Current Time + Compare ----------

struct timespec now_timespec(void);

// Return true if 'a' >= 'b' in timespec
bool timespec_ge(const struct timespec *a, const struct timespec *b);

// Add milliseconds to timespec
void timespec_add_ms(struct timespec *ts, int ms);
