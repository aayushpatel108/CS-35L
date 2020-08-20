#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, const char* argv[]) {

  const char* from = argv[1];
  const char* to = argv[2];

  if (argc < 3) {
    fprintf(stderr, "Error parsing arguemnts: Less than 2 arguments\n");
    exit(1);
  } else if (argc > 3) {
    fprintf(stderr, "Error parsing arguments: More than 2 arguments\n");
    exit(1);
  }
  int from_len = strlen(from);
  int to_len = strlen(to);

  if (from_len != to_len) {
    fprintf(stderr, "Error parsing arguments: Transliteration strings must be the same length\n");
    exit(1);
  }

    int i;
    int j;
    for (i = 0; i < from_len; i++)
        for (j = i + 1; j < from_len; j++) {
            if (from[j] == from[i]) {
                fprintf(stderr, "Error parsing arguments: Transliteration string cannot contain duplicate bytes\n");
                exit(1);
            }
        }

  char current_character[1];
  while (read(0, current_character, 1) > 0) {
    int i;
    for (i = 0; i < strlen(from); i++) {
      if (current_character[0] == from[i]) {
        current_character[0] = to[i];
        break;
      }
    }
    if (write(1, current_character, 1) < 0) {
        fprintf(stderr,"Error printing output");
        exit(1);
    }
  }
  return 0;

}