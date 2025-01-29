gcc -o vibe_time \
  main.c \
  ../common/console.c \
  ../common/script_reader.c \
  -I ../../quickjs/ \
  ../..//quickjs/libquickjs.a \
  -lm -lpthread
