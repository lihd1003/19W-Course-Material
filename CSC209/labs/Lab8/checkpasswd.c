#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXLINE 256
#define MAX_PASSWORD 10

#define SUCCESS "Password verified\n"
#define INVALID "Invalid password\n"
#define NO_USER "No such user\n"

int main(void) {
	char user_id[MAXLINE];
	char password[MAXLINE];
	int n;
	int fd[2];
  
	if(fgets(user_id, MAXLINE, stdin) == NULL) {
		perror("fgets");
		exit(1);
	}
	if(fgets(password, MAXLINE, stdin) == NULL) {
		perror("fgets");
		exit(1);
	}
  
	//create a pipe
	if (pipe(fd) == -1){
		perror("pipe");
		exit(1);
	}
  
	// create a child process
	if ((n = fork()) < 0){
		perror("fork");
		exit(1);
	}

	// parent process  
	if (n > 0){
	
		// close the reading pipe
		if ((close(fd[0])) == -1) {
			perror("close");
		}
	
		// write to the pipe
		if (write(fd[1], user_id, MAX_PASSWORD) == -1) {
			perror("write to pipe");
        }
		if (write(fd[1], password, MAX_PASSWORD) == -1) {
            perror("write to pipe");
        }
		
		// close the writing pipe
		if ((close(fd[1])) == -1) {
			perror("close");
		}
	
		// wait to get exit status from child
		int status;
		if (wait(&status) == -1){
			perror("wait");
			exit(1);
		}
		if (WIFEXITED(status)){
			if (WEXITSTATUS(status) == 0){
				printf(SUCCESS);
			} 
			else if (WEXITSTATUS(status) == 2){
				printf(INVALID);
			} 
			else if (WEXITSTATUS(status) == 3){
				printf(NO_USER);
			}
		} 
	} 
  
	// child process
	else if (n == 0){
	
		// close the writing pipe
		if ((close(fd[1])) == -1) {
			perror("close");
		}
	
		// redirect from stdin to the writing pipe
		if ((dup2(fd[0], fileno(stdin))) == -1) {
			perror("dup2");
			exit(1);
		}
		
		// execute validate
		execl("./validate", "validate", NULL);
  }
  
  
  


  return 0;
}
