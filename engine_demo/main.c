#include "../../quickjs/quickjs.h"
#include "console.h"
#include "script_reader.h"
#include <stdio.h>
#include <string.h>

// Custom JS function exposed that can be invoked from JS code
static JSValue js_vibetime(JSContext *ctx, JSValueConst this_val, int argc,
                           JSValueConst *argv) {
  printf("[C] Vibetime, it's an way of life!\n");

  // You can return a string, number, object, etc.
  // Here, let's return a simple string:
  return JS_NewString(ctx, "[JS] Vibetime is the right time!");
}

int main(int argc, char **argv) {
  // 1. Create a JS runtime and context
  JSRuntime *rt = JS_NewRuntime();
  if (!rt) {
    fprintf(stderr, "Failed to create JS runtime\n");
    return 1;
  }

  JSContext *ctx = JS_NewContext(rt);
  if (!ctx) {
    fprintf(stderr, "Failed to create JS context\n");
    JS_FreeRuntime(rt);
    return 1;
  }

  // 2. Create a global function named "vibetime"
  JSValue global_obj = JS_GetGlobalObject(ctx);
  JSValue func_val = JS_NewCFunction(ctx, js_vibetime, "vibetime", 0);
  JS_SetPropertyStr(ctx, global_obj, "vibetime", func_val);

  // 3. Configure the environment with a console object
  JSValue console_obj = JS_NewObject(ctx);
  JSValue log_fn = JS_NewCFunction(ctx, js_console_log, "log", 0);
  JS_SetPropertyStr(ctx, console_obj, "log", log_fn);
  JS_SetPropertyStr(ctx, global_obj, "console", console_obj);

  JS_FreeValue(ctx, global_obj);

  // 4. Read script from either file or stdin
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

  // 5. Send the script to the JS engine
  JSValue result_val =
      JS_Eval(ctx, script, strlen(script), "<input>", JS_EVAL_TYPE_GLOBAL);
  if (JS_IsException(result_val)) {
    // Print out any JS exceptions
    JSValue exception_val = JS_GetException(ctx);
    const char *error_str = JS_ToCString(ctx, exception_val);
    fprintf(stderr, "Uncaught exception: %s\n", error_str);
    JS_FreeCString(ctx, error_str);
    JS_FreeValue(ctx, exception_val);
  }
  JS_FreeValue(ctx, result_val);

  // 6. Cleanup
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);

  return 0;
}
