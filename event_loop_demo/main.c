#include "../../quickjs/quickjs.h"
#include "../common/console.h"
#include "../common/script_reader.h"
#include "timer.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// We'll keep a simple dynamically allocated array of timers
static TimerItem *g_timers = NULL;
static size_t g_timerCount = 0;

// ---------- The setTimeout Implementation ----------

static JSValue js_setTimeout(JSContext *ctx, JSValueConst this_val, int argc,
                             JSValueConst *argv) {
  if (argc < 2) {
    return JS_UNDEFINED;
  }

  // 1) The delay in milliseconds
  int64_t delay = 0;
  JS_ToInt64(ctx, &delay, argv[1]);

  // 2) The callback function
  if (!JS_IsFunction(ctx, argv[0])) {
    return JS_UNDEFINED;
  }

  // Duplicate the function so it stays valid
  JSValue func = JS_DupValue(ctx, argv[0]);

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

  return JS_UNDEFINED;
}

static void processTimers() {
  struct timespec now = now_timespec();
  size_t i = 0;

  // We'll check all timers for expiration
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
}

// ---------- The Event Loop ----------

static void runEventLoop(JSRuntime *rt, JSContext *ctx) {
  while (true) {
    processTimers();

    while (JS_ExecutePendingJob(rt, &ctx) > 0) {
      // Continue executing until no jobs remain
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
  runEventLoop(rt, ctx);

  // We'll never reach here in this sample
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);
  return 0;
}
