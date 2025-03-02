# JavaScript Customization Demos

This demo uses [QuickJS](https://bellard.org/quickjs/) to demonstrate the inner
workings of JavaScript engines.

## Disclaimer

> **⚠️ Warning:**  
> This project is intended **solely for demonstration purposes** and does
> **not** constitute production-ready code. It **does not** incorporate best
> practices, testing, comprehensive error handling, security measures, or
> performance optimizations required for deploying in a live environment. Use
> this code at your own risk and ensure thorough testing and necessary
> modifications before considering it for any production use.

## Build and Run the Demos

1. Get the source

   ```bash
   cd ~/src
   git clone git@github.com:dalealleshouse/javascript_customization.git
   git clone https://github.com/bellard/quickjs.git
   ```

1. Building QuickJS

   ```bash
   cd ~/src/quickjs
   make
   ```

1. Build and run the engine demo

   ```bash
   cd ~/src/javascript_customization/engine_demo/
   ./build.sh
   ./vibe_time ./vibe_time.js
   ```

1. Build and run the event loop demo

   ```bash
   cd ~/src/javascript_customization/event_loop_demo/
   ./build.sh
   ./vibe_loop ./vibe_loop.js
   ```
