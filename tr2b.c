#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char* argv[]) {

  const char* from = argv[1];
  const char* to = argv[2];

  if (argc < 3) {
    fprintf(stderr, "Error parsing arguemnts: Less than 2 arguments");
    exit(1);
  } else if (argc > 3) {
    fprintf(stderr, "Error parsing arguments: More than 2 arguments");
    exit(1);
  }
  int from_len = strlen(from);
  int to_len = strlen(to);

  if (from_len != to_len) {
    fprintf(stderr, "Error parsing arguments: Transliteration strings must be the same length");
    exit(1);
  }

    int i;
    int j;
    for (i = 0; i < from_len; i++)
        for (j = i + 1; j < from_len; j++) {
            if (from[j] == from[i]) {
                fprintf(stderr, "Error parsing arguments: Transliteration string cannot contain duplicate bytes");
                exit(1);
            }
        }

  int current_character = getchar();
  while (1) {
    if (ferror(stdin)) {
      fprintf(stderr,"Error reading input");
      exit(1);
    }
    if (feof(stdin) || current_character == EOF) {
        break;
    }
    int i;
    for (i = 0; i < from_len; i++) {
        if (current_character == from[i]) {
            current_character = to[i];
            break;
      } 
    }
    putchar(current_character);
    if (ferror(stdout)) {
        fprintf(stderr,"Error printing output");
        exit(1);
    }

    current_character = getchar();
  }

}