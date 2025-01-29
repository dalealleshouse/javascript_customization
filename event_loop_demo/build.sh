gcc -o vibe_loop \
  main.c \
  -I ../../quickjs/ \
  ../..//quickjs/libquickjs.a \
  -lm -lpthread
