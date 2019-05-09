#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

#include "freq_list.h"
#include "worker.h"

int main(int argc, char **argv) {
	
	// init, same as queryone
    char ch;
    char path[PATHLENGTH];
    char *startdir = ".";

    /* this models using getopt to process command-line flags and arguments */
    while ((ch = getopt(argc, argv, "d:")) != -1) {
        switch (ch) {
        case 'd':
            startdir = optarg;
            break;
        default:
            fprintf(stderr, "Usage: queryone [-d DIRECTORY_NAME]\n");
            exit(1);
        }
    }

    // Open the directory provided by the user (or current working directory)
    DIR *dirp;
    if ((dirp = opendir(startdir)) == NULL) {
        perror("opendir");
        exit(1);
    }

    struct dirent *dp;
	
	int fd_word[MAXWORKERS][2];
	int fd_fr[MAXWORKERS][2];
	
	int mark = 1;
	int child_index = 0;
	
	
	// set up all the processes and pipes
	while ((dp = readdir(dirp)) != NULL && mark > 0) {
		if (strcmp(dp->d_name, ".") == 0 ||	strcmp(dp->d_name, "..") == 0 ||
			strcmp(dp->d_name, ".svn") == 0 ||	strcmp(dp->d_name, ".git") == 0) 
			continue;
        
		strncpy(path, startdir, PATHLENGTH);
		strncat(path, "/", PATHLENGTH - strlen(path));
		strncat(path, dp->d_name, PATHLENGTH - strlen(path));
		path[PATHLENGTH - 1] = '\0';

		struct stat sbuf;
		if (stat(path, &sbuf) == -1) {
			// This should only fail if we got the path wrong
			// or we don't have permissions on this entry.
			perror("stat");
			exit(1);
		}
		
		
		if (S_ISDIR(sbuf.st_mode)){	
			if (child_index == MAXWORKERS){
				fprintf(stderr, "error: reach max_workers\n");
				exit(1);
			}
			// create pipes and child processes
			if (pipe(fd_word[child_index]) == -1){
				perror("create pipe");
				exit(1);
			}
			if (pipe(fd_fr[child_index]) == -1){
				perror("create pipe");
				exit(1);
			}	
			mark = fork();
			if (mark < 0){
				perror("fork");
				exit(1);
			}
			
			// close pipe ends for parent process
			if (mark > 0){
				if (close(fd_word[child_index][0]) == -1){
					perror("close pipe, parent word read");
					exit(1);
				}
				if (close(fd_fr[child_index][1]) == -1){
					perror("close pipe, parent fr write");
					exit(1);
				}
				child_index++;
			}
			
			// close pipe ends for child process
			else if (mark == 0){
				for (int i = 0; i <= child_index; i++){
					if (close(fd_word[i][1]) == -1){
						perror("close pipe, child word write");
						exit(1);
					}
					if (close(fd_fr[i][0]) == -1){
						perror("close pipe, child fr read");
						exit(1);
					}
				}
			}
		} 
	}
	
	// do the actual work
	if (mark > 0){
		
		if (child_index == 0){
			return 0;
		}
		char word[MAXWORD];
		FreqRecord sorted_fr[MAXRECORDS + 1];
		FreqRecord read_fr;
		while (fgets(word, MAXWORD, stdin) != NULL){
			sorted_fr[0].freq = 0;
			strcpy(sorted_fr[0].filename, "");
			sorted_fr[MAXRECORDS].freq = 0;
			strcpy(sorted_fr[MAXRECORDS].filename, "");
			for (int j = 0; j < child_index; j++){
				write(fd_word[j][1], word, MAXWORD);
			}
			for (int j = 0; j < child_index; j++){
				if (read(fd_fr[j][0], &read_fr, sizeof(FreqRecord)) == -1){
					perror("read");
					exit(1);
				}
				while (read_fr.freq > 0){
					update_sort(sorted_fr, read_fr, MAXRECORDS);
					if (read(fd_fr[j][0], &read_fr, sizeof(FreqRecord)) == -1){
						perror("read");
						exit(1);
					}
				}
			}
			print_freq_records(sorted_fr);
		}
		
	} 
	else if (mark == 0){
		run_worker(path, fd_word[child_index][0], fd_fr[child_index][1]);
	}
	
	// close the pipes
	if (mark > 0){
		for (int j = 0; j < child_index; j++){
			if (close(fd_fr[j][0]) == -1){
				perror("close");
				exit(1);
			}
			if (close(fd_word[j][1]) == -1){
				perror("close");
				exit(1);
			}
		}
	} else if (mark == 0){
		if (close(fd_word[child_index][0]) == -1){
			perror("close");
			exit(1);
		}
		if (close(fd_fr[child_index][1]) == -1){
			perror("close");
			exit(1);
		}
	}
    if (closedir(dirp) < 0)
        perror("closedir");

    return 0;
}
