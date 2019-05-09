/*
 * This program takes in no command-line arguments (if you put some in, they're
 * ignored).
 * It then reads in up to 30 characters from the keyboard (stopping early if you
 * press Enter), and prints that to standard output.
 */

#include <stdio.h>

int main() {
    char message[30];
    printf("Type in a message:\n");

    // Soon, you will learn how to check for errors from functions like fgets.
    fgets(message, 30, stdin);

    printf("Your input: %s\n", message);
    return 0;
}
