#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
    // Declare any variables you need

    
    // Write the code to loop over the command line arguments
    for (int i = 1; i < argc; i++) {
        // Call pipe before we fork



        // Call fork
        int result = fork();
		if (result < 0) {
			perror("fork");
			exit(1);

		} else if (result == 0) {
            // Child only writes to the pipe, so close reading end


            // Before we forked, parent had open the reading ends to
            // all previously forked children; so close those.
			



            // Now do the work - write the value in binary to the pipe
			int length = strlen(argv[i]);




            // Close the pipe since we are done with it.

            exit(0);

        } else {
            // In the parent, but before doing the next loop iteration,
            // close the end of the pipe that we don't want open

        }
    }
    
    // Only the parent gets here
    int sum = 0;
   
    // Read one integer from each child







    printf("Sum is %d\n", sum);
    return 0;
}

