#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv) {
    char *next = NULL;
    errno = 0;   // global variable has old value that might not be 0

    if(argc < 2) {
        printf("Usage: correctargs num\n");
        exit(1);
    }

    long longi = strtol(argv[1], &next, 0);
    int i = longi;

    printf("longi: %ld\n",longi);
    printf("i: %d\n",i);

    if (next == NULL) {
        printf("next is NULL\n");
    } else {
        printf("next is |%s|\n",next);
    }
    
    if(errno != 0) {
        perror("strtol");
    }

    return 0;
}
