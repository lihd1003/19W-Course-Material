#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "socket.h"

#ifndef PORT
  #define PORT 30000
#endif

#define BUFSIZE 30

int find_network_newline(const char *buf, int n);


int main() {
    // This line causes stdout not to be buffered.
    // Don't change this! Necessary for autotesting.
    setbuf(stdout, NULL);

    struct sockaddr_in *self = init_server_addr(PORT);
    int listenfd = set_up_server_socket(self, 5);

    while (1) {
        int fd = accept_connection(listenfd);
        if (fd < 0) {
            continue;
        }

        // Receive messages
        char buf[BUFSIZE] = {'\0'};
        int inbuf = 0;           // How many bytes currently in buffer?
        int room = sizeof(buf);  // How many bytes remaining in buffer?
        char *after = buf;       // Pointer to position after the data in buf

        int nbytes;
        while ((nbytes = read(fd, after, room)) > 0) {
            // Step 1: update inbuf (how many bytes were just added?)
			inbuf = inbuf + nbytes;
			
			
            int where;

            // Step 2: the loop condition below calls find_network_newline
            // to determine if a full line has been read from the client.
            // Your next task should be to implement find_network_newline
            // (found at the bottom of this file).
            //
            // Note: we use a loop here because a single read might result in
            // more than one full line.
            while ((where = find_network_newline(buf, inbuf)) > 0) {
                // Step 3: Okay, we have a full line.
                // Output the full line, not including the "\r\n",
                // using print statement below.
                // Be sure to put a '\0' in the correct place first;
                // otherwise you'll get junk in the output.
				buf[where - 2] = '\0';

                printf("Next message: %s\n", buf);
                // Note that we could have also used write to avoid having to
                // put the '\0' in the buffer. Try using write later!

                // Step 4: update inbuf and remove the full line from the buffer
                // There might be stuff after the line, so don't just do inbuf = 0.

                // You want to move the stuff after the full line to the beginning
                // of the buffer.  A loop can do it, or you can use memmove.
                // memmove(destination, source, number_of_bytes)
				inbuf = inbuf - where;
				memmove(&buf[0], &buf[where], inbuf);
				

            }
            // Step 5: update after and room, in preparation for the next read.
			after = &buf[inbuf];
			room = sizeof(buf) - inbuf;

        }
        close(fd);
    }

    free(self);
    close(listenfd);
    return 0;
}


/*
 * Search the first n characters of buf for a network newline (\r\n).
 * Return one plus the index of the '\n' of the first network newline,
 * or -1 if no network newline is found.
 * Definitely do not use strchr or other string functions to search here. (Why not?)
 */
int find_network_newline(const char *buf, int n) {
    for (int i = 0; i < n - 1; i++){
		if (buf[i] == '\r' && buf[i+1] == '\n'){
			return i + 2;
		}
	}
	return -1;
}

