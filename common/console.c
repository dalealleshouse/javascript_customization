#include "console.h"
#include "../../quickjs/quickjs.h"

JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc,
                       JSValueConst *argv) {
  // Print suffix to indicate this is a JS log
  printf("%s", "[JS] ");

  // Print all arguments separated by spaces
  for (int i = 0; i < argc; i++) {
    if (i > 0) {
      printf(" ");
    }
    const char *str = JS_ToCString(ctx, argv[i]);
    if (str) {
      printf("%s", str);
      JS_FreeCString(ctx, str);
    }
  }
  printf("\n");
  return JS_UNDEFINED;
}
