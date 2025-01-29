gcc -o vibe_time \
  main.c \
  console.c \
  script_reader.c \
  -I ../../quickjs/ \
  ../..//quickjs/libquickjs.a \
  -lm -lpthread
