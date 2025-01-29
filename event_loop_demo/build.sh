gcc -o vibe_loop \
  main.c \
  ../common/console.c \
  ../common/script_reader.c \
  -I ../../quickjs/ \
  ../..//quickjs/libquickjs.a \
  -lm -lpthread
