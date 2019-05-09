#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "hcq.h"

#ifndef PORT
  #define PORT 30000
#endif
#define MAX_BACKLOG 5
#define WRITE_BUF 1024

// Use global variables so we can have exactly one TA list and one student list
Ta *ta_list = NULL;
Student *stu_list = NULL;
Connection *connections = NULL;
Course *courses;  
int num_courses = 3;


// accept the connection from a client, init the Connection
// add it into the linked list
// write the welcome to the fd
int accept_connection(int fd, Connection **connections) {
	
	// init a new Connection
	Connection *new_connect = malloc(sizeof(Connection));
	if (new_connect == NULL){
		perror("malloc new connection");
		exit(1);
	}
	new_connect->state = 0;
	new_connect->ista = 2;
	new_connect->next = NULL;
	memset(new_connect->buf, '\0', 60);
	new_connect->inbuf = 0;
	new_connect->room = 60;
	new_connect->after = new_connect->buf;
	
	int client_fd = accept(fd, NULL, NULL);
	if (client_fd < 0) {
        perror("server: accept");
        close(fd);
        exit(1);
    }
	new_connect->fd = client_fd;
	
	
	if (*connections == NULL){
		*connections = new_connect;
	} else {
		Connection *curr = *connections;
		while (curr->next != NULL){
			curr = curr->next;
		}
		curr->next = new_connect;
	}
	char writebuf[WRITE_BUF];
	memset(writebuf, '\0', WRITE_BUF);
	strcpy(writebuf, "Welcome to the Help Centre, what is your name?\r\n");
	if (write(client_fd, writebuf, WRITE_BUF) != WRITE_BUF){
		perror("write");
		exit(1);
	}
	return client_fd;
}



// Search the first n characters of buf for a network newline (\r\n).
// Return one plus the index of the '\n' of the first network newline,
// or -1 if no network newline is found.
int find_network_newline(const char *buf, int n) {
    for (int i = 0; i < n - 1; i++){
		if (buf[i] == '\r' && buf[i+1] == '\n'){
			return i + 2;
		}
	}
	return -1;
}


// find a connection corresponding to a student given its name, 
// return its fd, or -1 if not found
int find_connection_by_name(char *name, Connection *connections){
	while (connections != NULL){
		if (strcmp(name, connections->name) == 0 && connections->ista == 0){
			return connections->fd;
		}
		connections = connections->next;
	}
	return -1;
}

// close the connection, return the connection ptr after it
void close_connection(int fd, Connection **connections){
	
	// find the corresponding Connection
	Connection *curr = *connections;
	while (curr != NULL) {
		if (curr->fd == fd){
			break;		
		}
		curr = curr->next;
	}
	
	if (curr == NULL){
		return;
	}

	// resolve the bonding ta/student data
	if (curr->state > 2){
		if (curr->ista == 1){
			remove_ta(&ta_list, curr->name);
		} else if (curr->ista == 0){
			give_up_waiting(&stu_list, curr->name);
		}
	}

	// resolve the linking
	//    if is the head
	if (*connections == curr){
		*connections = curr->next;
		free(curr);
		return;
	}
	//    if not the head
	Connection *prev = *connections;
	while (prev != NULL){
		if (prev->next == curr){
			prev->next = curr->next;
			break;
		}
		prev = prev->next;
	}
	
	free(curr);
}


// process the given information
// return the closed fd if any needed, otherwise return 0
int process_user(Connection *curr, char *toprocess){
	
	char writebuf[WRITE_BUF];
	memset(writebuf, '\0', WRITE_BUF);
	
	// get the name
	if (curr->state == 0){
		strcpy(curr->name, toprocess);
		(curr->state)++;
		strcpy(writebuf, "Are you a TA or a Student (enter T or S)?\r\n");
		if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
			return curr->fd;
		}
	} 
	
	// get the role
	else if (curr->state == 1){
		if (strcmp(toprocess, "T") == 0){
			curr->ista = 1;
			curr->state = 3;
			add_ta(&ta_list, curr->name);
			strcpy(writebuf, "Valid commands for TA:\r\n    stats\r\n    next\r\n    (or use Ctrl-C to leave)\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
		} 
		else if (strcmp(toprocess, "S") == 0){
			curr->ista = 0;
			(curr->state)++;
			strcpy(writebuf, "Valid courses: CSC108, CSC148, CSC209\r\nWhich course are your asking about?\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
		} 
		else {
			strcpy(writebuf, "Invalid role (enter T or S)\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
		}
	} 
	
	// get the student's course
	else if (curr->state == 2){
		int add_status = add_student(&stu_list, curr->name, toprocess, courses, 3);
		if (add_status == 0){
			curr->state = 3;
			strcpy(writebuf, "You have been entered into the queue. While you wait, you can use the command stats to see which TAs are currently serving students.\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
		} else if (add_status == 1){
			strcpy(writebuf, "This is not a valid course. Good-bye.\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
			close(curr->fd);
			return curr->fd;
		} else if (add_status == 2){
			strcpy(writebuf, "You are already in the queue and cannot be added again for any course. Good-bye.\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
			close(curr->fd);
			return curr->fd;
		} else {
			strcpy(writebuf, "Invalid syntax\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
		}
	} 
	
	// get the ta's command
	else if (curr->state == 3 && curr->ista == 1){
		if (strcmp(toprocess, "stats") == 0){
			char *buf = print_full_queue(stu_list);
			strncpy(writebuf, buf, WRITE_BUF);
			writebuf[WRITE_BUF - 1] = '\0';
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
			free(buf);
		} 
		else if (strcmp(toprocess, "next") == 0){
			if (stu_list != NULL){
				int fd = find_connection_by_name(stu_list->name, connections);
				char writebuf[WRITE_BUF];
				memset(writebuf, '\0', WRITE_BUF);
				strcpy(writebuf, "We are disconnecting you from the server now. Press Ctrl-C to close nc\r\n");
				if (write(fd, writebuf, WRITE_BUF) != WRITE_BUF){
					return curr->fd;
				}
				close(fd);
				next_overall(curr->name, &ta_list, &stu_list);
				return fd;
			}
			next_overall(curr->name, &ta_list, &stu_list);
		} 
		else {
			strcpy(writebuf, "Invalid syntax\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
		}
	} 
	
	// get the student's command
	else if (curr->state == 3 && curr->ista == 0){
		if (strcmp(toprocess, "stats") == 0){
			char *buf = print_currently_serving(ta_list);
			strncpy(writebuf, buf, WRITE_BUF);
			writebuf[WRITE_BUF - 1] = '\0';
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
			free(buf);
		} else {
			strcpy(writebuf, "Invalid syntax\r\n");
			if (write(curr->fd, writebuf, WRITE_BUF) != WRITE_BUF){
				return curr->fd;
			}
		}
	}
	
	return 0;
}

// read messages from the client_index and store into buf
// return the fd if it is closed, otherwise 0
int read_to_buf(Connection *curr) {

	int fd_close = 0;

	// read into the connection buf
	int nbytes;
	if ((nbytes = read(curr->fd, curr->after, curr->room - 1)) <= 0){
		return curr->fd;
	}
	
	curr->inbuf += nbytes;
	
	// see if the connection send something > 30 chars
	// if yes, close it
	int where;
	if ((where = find_network_newline(curr->buf, curr->inbuf)) == -1 && curr->inbuf > 30){
		close(curr->fd);
		return curr->fd;
	}
	
	// if successfully get one instruction, process it
	char toprocess[30] = "";
	while ((where = find_network_newline(curr->buf, curr->inbuf)) > 0){
		curr->buf[where - 2] = '\0';
		strcpy(toprocess, curr->buf);
		curr->inbuf -= where;
		memmove(&(curr->buf)[0], &(curr->buf)[where], curr->inbuf);
		if ((fd_close = process_user(curr, toprocess)) > 0){
			break;		
		}
	}
	
	curr->after = &(curr->buf)[curr->inbuf];
	curr->room = sizeof(curr->buf) - curr->inbuf;
	return fd_close;
}



int main(void) {
	
    // create courses
	if ((courses = malloc(sizeof(Course) * 3)) == NULL) {
        perror("malloc for course list\n");
        exit(1);
    }
    strcpy(courses[0].code, "CSC108");
    strcpy(courses[1].code, "CSC148");
    strcpy(courses[2].code, "CSC209");
	
    // Create the socket FD.
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("server: socket");
        exit(1);
    }

    // Set information about the port (and IP) we want to be connected to.
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    memset(&server.sin_zero, 0, 8);

	int on = 1;
    int status = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR,
                            (const char *) &on, sizeof(on));
    if(status == -1) {
        perror("setsockopt -- REUSEADDR");
    }
	
    // Bind the selected port to the socket.
    if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("server: bind");
        close(sock_fd);
        exit(1);
    }

    // Announce willingness to accept connections on this socket.
    if (listen(sock_fd, MAX_BACKLOG) < 0) {
        perror("server: listen");
        close(sock_fd);
        exit(1);
    }

    // The client accept - message accept loop. First, we prepare to listen to multiple
    // file descriptors by initializing a set of file descriptors.
    int max_fd = sock_fd;
    fd_set all_fds;
    FD_ZERO(&all_fds);
    FD_SET(sock_fd, &all_fds);

    while (1) {
        // select updates the fd_set it receives, so we always use a copy and retain the original.
        fd_set listen_fds = all_fds;
        int nready = select(max_fd + 1, &listen_fds, NULL, NULL, NULL);
        if (nready == -1) {
            perror("server: select");
            exit(1);
        }

        // Is it the original socket? Create a new connection ...
        if (FD_ISSET(sock_fd, &listen_fds)) {
            int client_fd = accept_connection(sock_fd, &connections);
            if (client_fd > max_fd) {
                max_fd = client_fd;
            }
            FD_SET(client_fd, &all_fds);
        }

        // Next, check the clients.
        // NOTE: We could do some tricks with nready to terminate this loop early.
		Connection *curr = connections;
		while (curr != NULL){
			if (FD_ISSET(curr->fd, &listen_fds)) {
				int client_closed = read_to_buf(curr);
           		if (client_closed > 0) {
            	    FD_CLR(client_closed, &all_fds);
					close_connection(client_closed, &connections);
					break;
            	}
			}
			curr = curr->next;	
		}
    }

    // Should never get here.
    return 1;
}
