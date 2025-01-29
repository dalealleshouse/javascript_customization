#include "../../quickjs/quickjs.h"
#include "../common/console.h"
#include "../common/script_reader.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // for usleep

// ---------- Timer Data Structures ----------

typedef struct {
  // When should this timer fire?
  struct timespec deadline;

  // The callback function stored as a JSValue
  JSValue callback;

  // We need the context to call the function
  JSContext *ctx;
} TimerItem;

// We'll keep a simple dynamically allocated array of timers
static TimerItem *g_timers = NULL;
static size_t g_timerCount = 0;

// ---------- Utility: Current Time + Compare ----------

static struct timespec now_timespec(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts;
}

// Return true if 'a' >= 'b' in timespec
static bool timespec_ge(const struct timespec *a, const struct timespec *b) {
  if (a->tv_sec > b->tv_sec)
    return true;
  if (a->tv_sec < b->tv_sec)
    return false;
  // same seconds, compare nanoseconds
  return (a->tv_nsec >= b->tv_nsec);
}

// Add milliseconds to timespec
static void timespec_add_ms(struct timespec *ts, int ms) {
  ts->tv_sec += ms / 1000;
  ts->tv_nsec += (ms % 1000) * 1000000L;
  if (ts->tv_nsec >= 1000000000L) {
    ts->tv_sec += 1;
    ts->tv_nsec -= 1000000000L;
  }
}

// ---------- The setTimeout Implementation ----------

static JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc,
                             JSValueConst *argv) {
  if (argc < 2) {
    return JS_UNDEFINED; // need delay + callback
  }
  // 1) The delay in milliseconds
  int64_t delay = 0;
  JS_ToInt64(ctx, &delay, argv[0]);

  // 2) The callback function
  if (!JS_IsFunction(ctx, argv[1])) {
    return JS_UNDEFINED;
  }

  // Duplicate the function so it stays valid
  JSValue func = JS_DupValue(ctx, argv[1]);

  // 3) Construct a TimerItem
  TimerItem newTimer;
  newTimer.ctx = ctx;
  newTimer.callback = func;
  newTimer.deadline = now_timespec();
  timespec_add_ms(&newTimer.deadline, delay);

  // 4) Insert into global array
  g_timers = realloc(g_timers, sizeof(TimerItem) * (g_timerCount + 1));
  g_timers[g_timerCount] = newTimer;
  g_timerCount++;

  printf("[C] New timer added, there are %zu total timers\n", g_timerCount);

  // Return undefined (we aren't returning a timer ID in this example)
  return JS_UNDEFINED;
}

// ---------- The Event Loop ----------

static void runEventLoop(void) {
  while (true) {
    struct timespec now = now_timespec();

    // We'll check all timers for expiration
    size_t i = 0;
    while (i < g_timerCount) {
      TimerItem *t = &g_timers[i];

      if (timespec_ge(&now, &t->deadline)) {
        // Timer is expired, call the JS function
        // We'll call with no arguments
        JSValue ret = JS_Call(t->ctx, t->callback, JS_UNDEFINED, 0, NULL);
        JS_FreeValue(t->ctx, ret);

        // Free the callback reference
        JS_FreeValue(t->ctx, t->callback);

        // Remove timer from array
        // Replace with last item for O(1) removal
        g_timerCount--;
        if (g_timerCount > 0 && i < g_timerCount) {
          g_timers[i] = g_timers[g_timerCount];
        }
        g_timers = realloc(g_timers, sizeof(TimerItem) * g_timerCount);

        printf("[C] Timer removed, there are %zu timers left\n", g_timerCount);
      } else {
        i++;
      }
    }

    // Sleep briefly to avoid 100% CPU usage
    usleep(10 * 1000); // 10ms
  }
}

int main(int argc, char **argv) {
  // 1. Create QuickJS runtime
  JSRuntime *rt = JS_NewRuntime();
  JSContext *ctx = JS_NewContext(rt);

  // 2. Provide console.log
  JSValue globalObj = JS_GetGlobalObject(ctx);
  JSValue consoleObj = JS_NewObject(ctx);
  JSValue logFn = JS_NewCFunction(ctx, js_console_log, "log", 0);
  JS_SetPropertyStr(ctx, consoleObj, "log", logFn);
  JS_SetPropertyStr(ctx, globalObj, "console", consoleObj);

  // 3. Provide setTimeout
  JSValue setTimeoutFn = JS_NewCFunction(ctx, js_setTimeout, "setTimeout", 2);
  JS_SetPropertyStr(ctx, globalObj, "setTimeout", setTimeoutFn);

  JS_FreeValue(ctx, globalObj);

  // 4. Let's run some sample JS to schedule timeouts
  const char *filename = NULL;
  if (argc > 1) {
    filename = argv[1];
  }

  char *script = read_entire_input(filename);
  if (!script) {
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    return 1;
  }

  // Evaluate the script
  JSValue val =
      JS_Eval(ctx, script, strlen(script), "<demo>", JS_EVAL_TYPE_GLOBAL);
  if (JS_IsException(val)) {
    // Print out any JS exceptions
    JSValue exc = JS_GetException(ctx);
    const char *errorStr = JS_ToCString(ctx, exc);
    fprintf(stderr, "Script Error: %s\n", errorStr);
    JS_FreeCString(ctx, errorStr);
    JS_FreeValue(ctx, exc);
  }
  JS_FreeValue(ctx, val);

  // 5. Start the event loop
  // This never exits in this demo
  runEventLoop();

  // We'll never reach here in this sample
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);
  return 0;
}
