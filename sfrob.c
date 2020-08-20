#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const *a, char const *b) {
    while (true) {
        if (*a == ' ') {
            return (*b == ' ') ? 0 : -1;
        } else if (*b == ' ') {
            return 1;
        }
        char unfrob_a = 0x2A ^ *a;
        char unfrob_b = 0x2A ^ *b;
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

int main()
{
    char character;

    char ** words = (char **)malloc(sizeof(char *));
    if(words == NULL){
        fprintf(stderr, "Error during memory allocation");
        exit(1);
    }
    char * word = (char *)malloc(sizeof(char));
    if(word == NULL){
        fprintf(stderr, "Error during memory allocation");
        exit(1);
    }
    int num_words = -1;
    int num_chars = 0;

    while (true)
    {
        character = getchar();
        if (ferror(stdin)) {
            fprintf(stderr, "I/O Error");
            exit(1);
        }
        if (feof(stdin)) {
            break;
        }
        
        if (character == ' ') {
            if (num_chars == 0) {
                continue;
            } else {
                word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
                if (word == NULL)
                {
                    fprintf(stderr, "Error during memory allocation");
                    exit(1);
                }
                num_words++;
                word[num_chars] = character;
                words = (char **)realloc(words, (num_words + 1) * sizeof(char *));
                if (words == NULL)
                {
                    fprintf(stderr, "Error during memory allocation");
                    exit(1);
                }
                words[num_words] = word;

                num_chars = 0;
                word = (char*)malloc(sizeof(char));
                if(word == NULL){
                    fprintf(stderr, "Error during memory allocation");
                    exit(1);
                }
            }
        } else {
            word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
            if (word == NULL)
            {
                fprintf(stderr, "Error during memory allocation");
                exit(1);
            }
            word[num_chars] = character;
            num_chars++;
        }
    }

    if (num_chars != 0) {
        word = (char *)realloc(word, (num_chars + 1) * sizeof(char));
        if(word == NULL){
                fprintf(stderr, "Error during memory allocation");
                exit(1);
            }
        word[num_chars] = ' ';
        num_words++;
        words = (char **)realloc(words, (num_words + 1) * sizeof(char *));
        if (words == NULL)
        {
            fprintf(stderr, "Error during memory allocation");
            exit(1);
        }
        words[num_words] = word;
    }

    qsort(words, num_words + 1, sizeof(char *), cmp);

    int i;
    for (i = 0; i < num_words + 1; i++) {
        int j = 0;
        while (true) {
            if (words[i][j] == ' ') {
                putchar(words[i][j]);
                break;
            }
            if (putchar(words[i][j]) == EOF) {
                fprintf(stderr, "Error during printing");
                exit(1);
            }
            j++;
        }
        free(words[i]);
    }
    free(words);
    exit(0);
}
