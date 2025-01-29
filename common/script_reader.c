#include "script_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char *read_entire_input(const char *filename) {
  FILE *fp = NULL;
  if (filename) {
    fp = fopen(filename, "rb");
    if (!fp) {
      fprintf(stderr, "Error: could not open file '%s'\n", filename);
      return NULL;
    }
  } else {
    fp = stdin;
    fprintf(stderr,
            "Reading JavaScript from stdin (press CTRL+D to finish)...\n");
  }

  // Find file size if not reading from stdin
  size_t size = 0;
  if (filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
      size = st.st_size;
    } else {
      fprintf(stderr, "Warning: could not determine file size.\n");
      size = 4096; // fallback
    }
  } else {
    // Arbitrary fallback size if reading from stdin
    size = 4096;
  }

  // Allocate buffer (add 1 for null terminator)
  char *buffer = malloc(size + 1);
  if (!buffer) {
    fprintf(stderr, "Error: not enough memory.\n");
    if (filename)
      fclose(fp);
    return NULL;
  }

  size_t bytesRead = fread(buffer, 1, size, fp);
  buffer[bytesRead] = '\0';

  if (filename)
    fclose(fp);
  return buffer;
}
