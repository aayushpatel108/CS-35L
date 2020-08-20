#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <ctype.h>

bool ignore_case;

int frobcmp(char const *a, char const *b) {
    while (true) {
        if (*a == ' ') {
            return (*b == ' ') ? 0 : -1;
        } else if (*b == ' ') {
            return 1;
        }
        char unfrob_a = 0x2A ^ *a;
        char unfrob_b = 0x2A ^ *b;
        if (ignore_case) {
            char unfrob_a = toupper ( (unsigned char) unfrob_a);
            char unfrob_b = toupper ( (unsigned char) unfrob_b);
        }
        if (unfrob_a < unfrob_b) {
            return -1;
        } else if (unfrob_a > unfrob_b) {
            return 1;
        } else {
            a++;
            b++;
        }
    }
    return 0;
}

int cmp(const void *a, const void *b) {
    return frobcmp(*(char **)a, *(char **)b);
}

bool parseOptions(const int argc, const char*argv[]) {
    if (argc == 2) {
        if (argv[1][0] == '-' && argv[1][1] == 'f') {
            return true;
        } else {
            fprintf(stderr, "Error: Invalid arguments.\n");
            exit(1);
        }
    } 
    if (argc > 2) {
        fprintf(stderr, "Error: Too many arguments.\n");
        exit(1);
    }
    return false;
}

int main(int argc, const char* argv[]) {
   ignore_case = parseOptions(argc, argv);
   struct stat buffer;
   fstat(0, &buffer);
   size_t file_size;
   bool addNewString = true;

   if (fstat(0, &buffer) < 0) {
    fprintf(stderr, "Error during memory allocation: buffer error\n");
    exit(1);
   }

    char* regular_file;
    char character;
    char ** words = NULL;
    int num_words = -1;
    int num_chars = 0;

    if (S_ISREG(buffer.st_mode)) {
        file_size = buffer.st_size;
        regular_file = (char*) malloc(sizeof(char) * (file_size+1));
        if (read(0, regular_file, file_size) < 0) {
            fprintf(stderr, "Error during memory allocation: buffer error\n");
            exit(1);
        }
        int word_count = 0;
        int i;
        for (i = 0; i < file_size; i++) {
            if (regular_file[i] == ' ') {
                while (regular_file[i] == ' ' && i < file_size) i++;
                if (i < file_size) word_count++;
            } else {
                if (i == 0) word_count++;
            }
        }
        regular_file[file_size] = ' ';
        words = (char**) malloc(sizeof(char*) * word_count);
        if (words == NULL) {
            fprintf(stderr, "Error during memory allocation1\n");
            exit(1);
        }
        char* word;
        for(i = 0; i < file_size+1; i++){
            character = regular_file[i];
            if (character != ' ') {
                word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
                if (word == NULL) {
                    fprintf(stderr, "Error during memory allocation10\n");
                    exit(1);
                }
                word[num_chars] = character;
                num_chars++;
            } else {
                if (num_chars == 0) {
                    continue;
                } else {
                    word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
                    if (word == NULL) {
                        fprintf(stderr, "Error during memory allocation11\n");
                        exit(1);
                    }
                    word[num_chars] = character;
                    num_chars++;
                    num_words++;
                    words[num_words] = word;
                    num_chars = 0;
                    word = (char *)malloc(sizeof(char));
                    if (word == NULL){
                        fprintf(stderr, "Error during memory allocation12\n");
                        exit(1);
                    }
                }
            }
        } 
    } else {
        words = (char**) malloc(sizeof(char*));
        if (words == NULL) {
            fprintf(stderr, "Error during memory allocation2\n");
            exit(1);
        }
        char * word = (char *)malloc(sizeof(char));
        if (word == NULL){
            fprintf(stderr, "Error during memory allocation3\n");
            exit(1);
        }
        char input[1];
        while (true) {
            int read_input = read(0, input, 1);
            if (read_input == 0) {
                break;
            } else if (read_input < 0) {
                fprintf(stderr, "I/O Error");
                exit(1);
            }
            char character = input[0];
            if (character == EOF) break;
            if (character == ' ') {
                if (num_chars == 0) {
                    continue;
                } else {
                    word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
                    if (word == NULL) {
                        fprintf(stderr, "Error during memory allocation4");
                        exit(1);
                    }
                    num_words++;
                    word[num_chars] = character;
                    words = (char **)realloc(words, (num_words + 1) * sizeof(char *));
                    if (words == NULL) {
                        fprintf(stderr, "Error during memory allocation5");
                        exit(1);
                    }
                    words[num_words] = word;
                    num_chars = 0;
                    word = (char*)malloc(sizeof(char));
                    if(word == NULL){
                        fprintf(stderr, "Error during memory allocation6");
                        exit(1);
                    }
                }
            } else {
                word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
                if (word == NULL) {
                    fprintf(stderr, "Error during memory allocation7");
                    exit(1);
                }
                word[num_chars] = character;
                num_chars++;
            }
        }
        if (num_chars != 0) {
            word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
            if(word == NULL){
                    fprintf(stderr, "Error during memory allocation8");
                    exit(1);
            }
            word[num_chars] = ' ';
            num_words++;
            words = (char **)realloc(words, (num_words + 1) * sizeof(char *));
            if (words == NULL) {
                fprintf(stderr, "Error during memory allocation9");
                exit(1);
            }
            words[num_words] = word;
        }
   }
    qsort(words, num_words + 1, sizeof(char *), cmp);
    int i;
    int j;
    char output[1];
    for (i = 0; i < num_words + 1; i++) {
        j = 0;
        while (true) {
            output[0] = words[i][j];
            if (words[i][j] == ' ') {
                write(1, output, 1);
                break;
            }
            if (write(1, output, 1) < 0) {
                fprintf(stderr, "Error during printing");
                exit(1);
            }
            j++;
        }
        free(words[i]);
    }
    if (S_ISREG(buffer.st_mode)) {
        free(regular_file);
    }
    free(words);
    exit(0);
}
