#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "hcq.h"

#define INPUT_BUFFER_SIZE 256
#define INPUT_ARG_MAX_NUM 3  
#define DELIM " \n"


/* Print a formatted error message to stderr.
 */
void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

// Use global variables so we can have exactly one TA list and one student list
Ta *ta_list = NULL;
Student *stu_list = NULL;

Course *courses;  
int num_courses = 3;

/* 
 * Read and process commands
 * Return:  -1 for quit command
 *          0 otherwise
 */
int process_args(int cmd_argc, char **cmd_argv) {

    int result;

    if (cmd_argc <= 0) {
        return 0;
    } else if (strcmp(cmd_argv[0], "add_student") == 0 && cmd_argc == 3) {
        result = add_student(&stu_list, cmd_argv[1], cmd_argv[2], courses,
                        num_courses); 
        if (result == 1) {
            error("This student is already in the queue.");
        } else if (result == 2) {
            error("Invalid Course -- student not added.");
        }
    }  else if (strcmp(cmd_argv[0], "print_full_queue") == 0 && cmd_argc == 1) {
        char *buf = print_full_queue(stu_list);
		printf("%s", buf);
		free(buf);

    } else if (strcmp(cmd_argv[0], "print_currently_serving") == 0 && cmd_argc == 1) {
		char *buf = print_currently_serving(ta_list);
        printf("%s", buf);
		free(buf);

    } else if (strcmp(cmd_argv[0], "give_up") == 0 && cmd_argc == 2) {
        if (give_up_waiting(&stu_list, cmd_argv[1]) == 1) {
            error("There was no student by that name waiting in the queue.");
        }
    } else if (strcmp(cmd_argv[0], "add_ta") == 0 && cmd_argc == 2) {
        add_ta(&ta_list, cmd_argv[1]);

    } else if (strcmp(cmd_argv[0], "remove_ta") == 0 && cmd_argc == 2) {
        if (remove_ta(&ta_list, cmd_argv[1]) == 1) {
           error("Invalid TA name.");
        }
    } else if (strcmp(cmd_argv[0], "next") == 0 && cmd_argc == 2) {
        if (next_overall(cmd_argv[1], &ta_list, &stu_list) == 1) {;
           error("Invalid TA name.");
        }
    } else {
        error("Incorrect syntax.");
    }
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc < 1 || argc > 2) {
        fprintf(stderr, "Usage: ./helpcentre [commands_filename]\n");
        exit(1);
    }
    int batch_mode = (argc == 3);
    char input[INPUT_BUFFER_SIZE];
    FILE *input_stream;

    if ((courses = malloc(sizeof(Course) * 3)) == NULL) {
        perror("malloc for course list\n");
        exit(1);
    }
    strcpy(courses[0].code, "CSC108");
    strcpy(courses[1].code, "CSC148");
    strcpy(courses[2].code, "CSC209");

    // for holding arguments to individual commands passed to sub-procedure
    char *cmd_argv[INPUT_ARG_MAX_NUM];
    int cmd_argc;

    if (batch_mode) {
        input_stream = fopen(argv[2], "r");
        if (input_stream == NULL) {
            perror("Error opening file");
            exit(1);
        }
    } else { // interactive mode 
        input_stream = stdin;
    }

    printf("Welcome to the Help Centre Queuing System\nPlease type a command:\n>");
    
    while (fgets(input, INPUT_BUFFER_SIZE, input_stream) != NULL) {
        // only echo the line in batch mode since in interactive mode the user
        // has just typed the line
        if (batch_mode) {
            printf("%s", input);
        }
        // tokenize arguments
        // Notice that this tokenizing is not sophisticated enough to 
        // handle quoted arguments with spaces so names can not have spaces.
        char *next_token = strtok(input, DELIM);
        cmd_argc = 0;
        while (next_token != NULL) {
            if (cmd_argc >= INPUT_ARG_MAX_NUM) {
                error("Too many arguments.");
                cmd_argc = 0;
                break;
            }
            cmd_argv[cmd_argc] = next_token;
            cmd_argc++;
            next_token = strtok(NULL, DELIM);
        }
        if (cmd_argc > 0 && process_args(cmd_argc, cmd_argv) == -1) {
            break; // can only reach if quit command was entered
        }
        printf(">");
    }

    if (batch_mode) {
        fclose(input_stream);
    }
    return 0;
}
