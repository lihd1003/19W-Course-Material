#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

char *name;


void sing(int code) {
    fprintf(stderr, "Happy birthday to you\n");
    fprintf(stderr, "Happy birthday to you\n");
    sleep(7);
    fprintf(stderr, "Happy birthday dear %s\n", name);
    fprintf(stderr, "Happy birthday to you\n");
}


void dance(int code) {
    printf("Dance dance dance\n");
}


int main(int argc, char **argv) {
    name = argv[1];

    // install signal handler 
    struct sigaction sa;
    sa.sa_handler = sing;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);

    struct sigaction sa2;
    sa2.sa_handler = dance;
    sa2.sa_flags = 0;
    sigemptyset(&sa2.sa_mask);
    sigaction(SIGUSR2, &sa2, NULL);

    sigset_t blocked_set;
    sigemptyset(&blocked_set);
    sigaddset(&blocked_set, SIGUSR2);
    for (;;) {
       if (sigprocmask(SIG_BLOCK, &blocked_set, NULL) == -1) {
           perror("sigprocmask");
           exit(1);
       }
       printf("I'm too tired to dance now\n");
       sleep(6);
       printf("ok I'm ready to dance again\n");
       if (sigprocmask(SIG_UNBLOCK, &blocked_set, NULL) == -1) {
           perror("sigprocmask");
           exit(1);
       }
       sleep(6);
    }

    return 0;
}
