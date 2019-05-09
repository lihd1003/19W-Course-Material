#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "hcq.h"

#define INPUT_BUFFER_SIZE 256
#define INPUT_ARG_MAX_NUM 3  
#define DELIM " \n"
#define CONFIG_FILENAME "courses.config"


/* Print a formatted error message to stderr.
 */
void error(char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
}

/*
 * Displays the available commands and their options.
 */
void display_help_message() {
    printf("help\n");
    printf("quit\n");
    printf("add_student name course\n");
    printf("print_full_queue\n");
    printf("print_currently_serving\n");
    printf("print_all_queues\n");
    printf("stats_by_course course\n");
    printf("add_ta name\n");
    printf("remove_ta name\n");
    printf("give_up student_name\n");
    printf("next ta_name [course]\n");
    fflush(stdout);
}

/* 
 * Read and process commands
 * Return:  -1 for quit command
 *          0 otherwise
 */
int process_args(int cmd_argc, char **cmd_argv, 
                 Student **stu_list_ptr, 
                 Ta **ta_list_ptr,
                 Course *courses, int num_courses) {

    Student *stu_list = *stu_list_ptr; 
    Ta *ta_list = *ta_list_ptr;
    int result;

    if (cmd_argc <= 0) {
        return 0;
    } else if (strcmp(cmd_argv[0], "quit") == 0 && cmd_argc == 1) {
        return -1;
    } else if (strcmp(cmd_argv[0], "help") == 0 && cmd_argc == 1) {
        display_help_message();
    } else if (strcmp(cmd_argv[0], "sleep") == 0 && cmd_argc == 1) {
        sleep(2);
    } else if (strcmp(cmd_argv[0], "add_student") == 0 && cmd_argc == 3) {
        result = add_student(stu_list_ptr, cmd_argv[1], cmd_argv[2], courses,
                        num_courses); 
        if (result == 1) {
            error("This student is already in the queue.");
        } else if (result == 2) {
            error("Invalid Course -- student not added.");
        }
    }  else if (strcmp(cmd_argv[0], "print_full_queue") == 0 && cmd_argc == 1) {
        print_full_queue(stu_list);

    } else if (strcmp(cmd_argv[0], "print_currently_serving") == 0 && cmd_argc == 1) {
        print_currently_serving(ta_list);

    } else if (strcmp(cmd_argv[0], "print_all_queues") == 0 && cmd_argc == 1) {
        print_all_queues(stu_list, courses, num_courses);

    } else if (strcmp(cmd_argv[0], "stats_by_course") == 0 && cmd_argc == 2) {
        if (stats_by_course(stu_list, cmd_argv[1], courses, num_courses, ta_list) == 1) {
            error("Invalid course code.");
        }
    } else if (strcmp(cmd_argv[0], "give_up") == 0 && cmd_argc == 2) {
        if (give_up_waiting(stu_list_ptr, cmd_argv[1]) == 1) {
            error("There was no student by that name waiting in the queue.");
        }
    } else if (strcmp(cmd_argv[0], "add_ta") == 0 && cmd_argc == 2) {
        add_ta(ta_list_ptr, cmd_argv[1]);
    } else if (strcmp(cmd_argv[0], "remove_ta") == 0 && cmd_argc == 2) {
        if (remove_ta(ta_list_ptr, cmd_argv[1]) == 1) {
           error("Invalid TA name.");
        }
    } else if (strcmp(cmd_argv[0], "next") == 0 && cmd_argc == 2) {
        if (take_next_overall(cmd_argv[1], ta_list, stu_list_ptr) == 1) {;
           error("Invalid TA name.");
        }
    } else if (strcmp(cmd_argv[0], "next") == 0 && cmd_argc == 3) {
        result = take_next_course(cmd_argv[1], ta_list, stu_list_ptr, 
                                      cmd_argv[2], courses, num_courses);
        if (result == 1) {
           error("Invalid TA name.");
        } else if (result == 2) {
            error("Invalid course code.");
        }
    } else {
        error("Incorrect syntax.");
    }
    return 0;
}


int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: ./helpcentre config_filename [commands_filename]\n");
        exit(1);
    }
    int batch_mode = (argc == 3);
    char input[INPUT_BUFFER_SIZE];
    FILE *input_stream;

    // for holding arguments to individual commands passed to sub-procedure
    char *cmd_argv[INPUT_ARG_MAX_NUM];
    int cmd_argc;

    // Create the heads of the two empty data-structures for Students and Tas
    Ta *ta_list = NULL;
    Student *stu_list = NULL;

    // Call configure_course_list to set up the course list
    Course *courses; 
    int num_courses = config_course_list(&courses, argv[1]);

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
        if (cmd_argc > 0 && process_args(cmd_argc, cmd_argv, 
                            &stu_list, &ta_list, courses, num_courses) == -1) {
            break; // can only reach if quit command was entered
        }
        printf(">");
    }

    if (batch_mode) {
        fclose(input_stream);
    }
    return 0;
}
