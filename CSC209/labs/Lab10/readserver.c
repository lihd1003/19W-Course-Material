#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "socket.h"

#ifndef PORT
  #define PORT 30000
#endif

#define BUFSIZE 30


int main() {
    struct sockaddr_in *self = init_server_addr(PORT);
    int listenfd = set_up_server_socket(self, 5);

    while (1) {
        int fd = accept_connection(listenfd);
        if (fd < 0) {
            continue;
        }

        // Receive messages
        char buf[BUFSIZE];
        int nbytes;
        while ((nbytes = read(fd, buf, sizeof(buf) - 1)) > 0) {
            buf[nbytes] = '\0';
            printf("Next message: %s\n", buf);
        }
        close(fd);
    }

    free(self);
    close(listenfd);
    return 0;
}
