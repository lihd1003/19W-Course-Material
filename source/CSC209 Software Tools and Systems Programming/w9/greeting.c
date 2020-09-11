#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

char* name;

void sing(int sig) {
    printf("Happy birthday to you,\n");
    printf("Happy birthday to you,\n");
    sleep(10);
    printf("Happy birthday dear %s,\n", name);
    printf("Happy birthday to you!\n");

}

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: greeting name\n");
        exit(1);
    }
    name = argv[1];

    // Declare a struct to be used by the sigaction function:
    struct sigaction newact;

    // Specify that we want the handler function to handle the
    // signal:
    newact.sa_handler = sing;

    // Use default flags:
    newact.sa_flags = 0;

    // Specify that we don't want any signals to be blocked during
    // execution of handler:
    sigemptyset(&newact.sa_mask);
    sigaddset(&newact.sa_mask, SIGINT);

    // Modify the signal table so that handler is called when
    // signal SIGUSR1 is received:
    if (sigaction(SIGUSR1, &newact, NULL) == -1) {
        perror("sigacction");
        exit(1);
    };


    while(1) {        
        // could print some dots to stderr so we can see that we are running
        // use stderr, so buffering isn't an issue
        fprintf(stderr, ".");
        sleep(1);
    }
    return 0;
}
