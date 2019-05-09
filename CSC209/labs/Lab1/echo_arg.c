/*
 * This program takes in one command-line argument.
 * It then echoes that command-line argument back to stdout.
 */

#include <stdio.h>

int main(int argc, char **argv) {

    if (argc == 1) {
        printf("Usage: %s ARG\n", argv[0]);
        return 1;
    }
    printf("Your command-line argument: %s\n", argv[1]);
    return 0;
}
