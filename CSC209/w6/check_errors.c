#include <stdio.h>
#include <stdlib.h>

void *Malloc(int size) {
    void *result = malloc(size);
    if (result == NULL) {
        perror("malloc");
        exit(1);
    }
    return result;
}

int main(int argc, char **argv) {
    
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    int num;
    if (fread(&num, sizeof(int), 1, fp) == 0) {
        fprintf(stderr, "not enough to read\n"); 
    } else {
        printf("I read %d\n", num);
    }

    
    char *str; 
    str = Malloc(sizeof(char) * 1024);
    
    return 0;
}
