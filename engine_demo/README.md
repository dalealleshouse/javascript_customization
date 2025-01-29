# Quick JS

This demo uses [QuickJS](https://bellard.org/quickjs/) to demonstrate how
JavaScript engines work.

1. Get the source

   ```bash
   cd ~/src
   git clone git@github.com:dalealleshouse/javascript_customization.git
   git clone https://github.com/bellard/quickjs.git
   ```

1. Build QuickJS

   ```bash
   cd ~/src/quickjs
   make
   ```

1. Build the Demo

   ```bash
   cd ~/src/javascript_customization/engine_demo/
   ./build.sh
   ```

1. Run the Code

   ```bash
   ./vibe_time
   ```
