#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>    /* Internet domain header */
#include <arpa/inet.h>     /* only needed on my mac */

#define LEGO_PIECES 10;

// Helper function: accept a new player
int accept_player(int listen_soc, char *name) {
    struct sockaddr_in client_addr;
    unsigned int client_len = sizeof(struct sockaddr_in);
    client_addr.sin_family = AF_INET;

    int client_socket = accept(listen_soc, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("accept");
        return -1;
    } 

    dprintf(client_socket, "Hello player %s! Please wait for your turn to begin.\r\n", name);

    return client_socket;
}

// Helper function: write to all players
void write_to_players(char *msg, int *player, int num_player) {
    for(int i = 0; i < num_player; i++) {
        write(player[i], msg, strlen(msg));
    }
}

// Helper function: read a move from a player.
// A valid move is a integer between 1-3.
// It returns -1 if an error is occured.
int read_a_move(int player) {
    // Read a line from the player
    char line[10];
    memset(line, '\0', 10);
    int move = -1;
    while (read(player, line, 10) > 0) {
        char *ptr;
        if ((ptr = strstr(line, "\r\n")) != NULL) {
            *ptr = '\0';
            *(ptr + 1) = '\0';
        }

        move = strtol(line, NULL, 10);
        if (move >= 1 && move <=3) {
            break;
        } else if (strlen(line) > 0) {
            dprintf(player, "your move is not allowed. Please enter a move between 1-3.\r\n");
        }
    }

    return move;
}

int main() {
    // create socket
    int listen_soc = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_soc == -1) {
        perror("server: socket");
        exit(1);
    }

    // initialize server address    
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(56224);  
    memset(&server.sin_zero, 0, 8);
    server.sin_addr.s_addr = INADDR_ANY;

    // recycle port right away
    int on = 1;
    int status = setsockopt(listen_soc, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on));
    if (status == -1) {
        perror("setsockopt -- REUSEADDR");
    }

    // bind socket to an address
    if (bind(listen_soc, (struct sockaddr *) &server, sizeof(struct sockaddr_in)) == -1) {
      perror("server: bind");
      close(listen_soc);
      exit(1);
    } 

    // Set up a queue in the kernel to hold pending connections.
    if (listen(listen_soc, 5) < 0) {
        // listen failed
        perror("listen");
        exit(1);
    }

    // Accept two players
    int player[2] = {accept_player(listen_soc, "one"), accept_player(listen_soc, "two")};

    // Game play
    int item = LEGO_PIECES;
    int round = 0;
    while (item > 0) {
        // Announce the current status to all players
        char status_update[128] = "There are %d lego pieces left.\r\n";
        sprintf(status_update, status_update, item);
        write_to_players(status_update, player, 2);

        // Read a move from one player. Alternating between two players.
        int curr_player = player[round % 2];
        dprintf(curr_player, "Please enter a move between 1-3.\r\n");
        int move = read_a_move(curr_player);
        item -= move;
        round += 1;

        // Prompt the player to wait
        dprintf(curr_player, "Thanks! Please wait for the other player to move.\r\n");
    }

    char winner_announcement[128] = "Winner is player %d!\r\n";
    sprintf(winner_announcement, winner_announcement, (round - 1) % 2 + 1);
    write_to_players(winner_announcement, player, 2);

    return 0;
}


