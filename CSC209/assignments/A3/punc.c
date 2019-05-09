#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *remove_punc(char *word) {
    char *result;
    int i = 0;
    int j = 0;
    int len = strlen(word);

    if ((result = malloc(len + 1)) == NULL) {
        perror("malloc");
        exit(1);
    }

    /* remove punctuation from the beginning of the word */
    while (i < len && ispunct(word[i])) {
        i++;
    }

    while (word[i] != '\0') {
        result[j] = tolower(word[i]);
        j++;
        i++;
    }
    result[j] = '\0';

    /* remove punctuation from the end of the word */
    i = strlen(result) - 1;
    while (i >= 0 && (ispunct(result[i]) || isspace(result[i]))) {
        result[i] = '\0';
        i--;
    }
    return result;
}
