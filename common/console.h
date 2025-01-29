#pragma once

#include "../../quickjs/quickjs.h"

JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc,
                       JSValueConst *argv);
