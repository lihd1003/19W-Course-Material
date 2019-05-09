#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// NOTE: MAX_PASSWORD must be less than  MAXLINE/2
#define MAXLINE 32
#define MAX_PASSWORD 10  

#define PASSWORD_FILE "pass.txt"

/* Reads two chunks from stdin, and checks if they match a user id
 * and password pair from a password file. The first chunk (MAX_PASSWORD bytes)
 * will contain a user id, and the second chunk (MAX_PASSWORD bytes) will contain 
 * a password.
 * 
 * The program exits with a value of 
 *      0 if the user id and password match,
 *      1 if there is an error, 
 *      2 if the user id is found but the password does not match, and 
 *      3 if the user id is not found in the password file. 
 */

int main(void){
    int n, user_length;
    char userid[MAXLINE];
    char password[MAXLINE];

    if ((n = read(STDIN_FILENO, userid, MAX_PASSWORD)) == -1) {
        perror("read");
        exit(1);
    } else if(n == 0) {
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    }

    // Make sure user id is null-terminated
    if(n <= MAX_PASSWORD) {
        userid[n] ='\0';
    }

    // Remove newline character if it exists
    char *newline;
    if((newline=strchr(userid, '\n')) != NULL) {
        *newline = '\0';
    }

    if ((n = read(STDIN_FILENO, password, MAX_PASSWORD)) == -1) {
        perror("read");
        exit(1);
    } else if (n == 0) {
        fprintf(stderr, "Error: could not read from stdin");
        exit(1);
    }

    // Make sure password is null-terminated
    if(n <= MAX_PASSWORD) {
        password[n] ='\0';
    }

    // Remove newline character if it exists
    if((newline = strchr(password, '\n')) != NULL) {
        *newline = '\0';
    }

    // We expect userid to have enough space to concatenate ":" + password
    // but we will play it safe and use strncat
    strncat(userid, ":", MAXLINE - strlen(userid) - 1);
    user_length = strlen(userid);
    strncat(userid, password, MAXLINE - strlen(userid) - 1);

    FILE *fp = fopen(PASSWORD_FILE, "r");
    if (!fp) {
        perror("fopen");
        exit(1);
    }
    char line[MAXLINE];
    while(fgets(line, sizeof(line) - 1, fp)) {
        line[strlen(line) - 1] = '\0';
        if (strcmp(userid, line) == 0) {
            exit(0); // found match
        }
        else if(strncmp(userid, line, user_length) == 0)
            exit (2); // invalid password
    }
    exit(3); // no such user
}
