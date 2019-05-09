#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/* Write random integers (in binary) to a file with the name given by the command-line
 * argument.  This program creates a data file for use by the time_reads program.
 */

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: write_test_file filename\n");
        exit(1);
    }

    FILE *fp;
    if ((fp = fopen(argv[1], "w")) == NULL) {
        perror("fopen");
        exit(1);
    }

	int n_array[100]; 
    // TODO: complete this program according its description above.
	for (int i = 0; i < 100; i++){
		n_array[i] = random() % 100;
	}
	fwrite(n_array, sizeof(int), 100, fp);

    fclose(fp);
    return 0;
}
