#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "socket.h"

#ifndef PORT
  #define PORT 30000
#endif

#define MESSAGE "A stitch in time\r\n"
#define TIMES 5 // number of times to send the message
#define MINCHARS 3
#define MAXCHARS 7


void write_random_pieces(int soc, const char *message, int times);


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: randclient hostname\n");
        exit(1);
    }

    int soc = connect_to_server(PORT, argv[1]);

    write_random_pieces(soc, MESSAGE, TIMES);

    close(soc);
    return 0;
}


/*
 * Write the given message the given number of times to the given socket, and randomly-sized pieces.
 * Do NOT write any null-terminator characters.
 */
void write_random_pieces(int soc, const char *message, int times) {
    char piece[MAXCHARS];
    int message_len = strlen(message);
    int total_bytes = times * message_len;
    int current_byte = 0;

    while (current_byte < total_bytes) {
        int piece_size = rand() % (MAXCHARS - MINCHARS + 1) + MINCHARS;
        int bytes_left = total_bytes - current_byte;
        if (piece_size > bytes_left) {
            piece_size = bytes_left;
        }

        for (int i = 0; i < piece_size; i++) {
            piece[i] = message[(current_byte + i) % message_len];
        }
        write(soc, piece, piece_size);
        current_byte += piece_size;
    }
}
