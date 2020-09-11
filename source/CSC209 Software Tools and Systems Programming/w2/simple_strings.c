#include <stdio.h>
#include <string.h> // we added this to be able to call strcmp and strcpy
#include <stdlib.h> // we added this to be able to call exit

int main(int argc, char **argv) {

    // Initializing a character array to be a string
    //  - we need one character to hold the null-terminator ('\0') that 
    //    indicates the end of the string
    char target[4] = "yes";

    // Assigning to a string  
    //   - can NOT do   target = "no";
    //     Instead we copy into the array. This copies 3 characters
    //     into the target 'n' 'o' and '\0'
    strcpy(target,"no");

    // Before we added this, we got a segfault when called with too few args 
    if (argc != 3) {
        // We print the error message to stderr rather than the default 
        // stdout (used by printf) because we want to see the error message
        // on a terminal even when we have redirected stdout.
        fprintf(stderr, "Call me with 2 arguments\n");
        exit(1);   // completely quits running the program and returns 1 to
                   // the shell. Since we are in main, this has the same 
                   // effect as return 1;
    }

    // We compare strings using strcmp NOT ==
    // strcmp returns 0 when they match and non-zero otherwise
    if (strcmp(argv[1], argv[2]) == 0) {
        printf("args match\n");
    } else {
        printf("args do not match\n");
    }
    // You will often see this form as well. Since 0 is false, we use 
    // ! (for not) to get true when the arg and target are the same
    if (!strcmp(argv[1], target)) {
        printf("arg1 is target\n");
    }
    return 0;
}
