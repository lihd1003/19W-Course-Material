#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include "freq_list.h"
#include "worker.h"

/* given a word and a linked list of indexs and an array of filenames
   returns a pointer to an array of FreqRecord
*/
FreqRecord *get_word(char *word, Node *head, char **file_names) {
	
	FreqRecord *ret = malloc(sizeof(FreqRecord) * MAXFILES);
	if (ret == NULL){
		perror("create FreqRecord");
		exit(1);
	}

	// find the word that match
	Node *curr = head;
	while (curr != NULL) {
		if (strcmp(curr->word, word) == 0){
			break;
		}
		curr = curr->next;
	}
	
	// fill in the freqrecord
	int index = 0;
	if (curr != NULL){
		for (int i = 0; i < MAXFILES - 1; i++){
			if (curr->freq[i] != 0){
				ret[index].freq = curr->freq[i];
				strcpy(ret[index].filename, file_names[i]);
				index++;
			}
		}
	}
	ret[index].freq = 0;
	strcpy(ret[index].filename, "");
	
	return ret;
}

/* Print to standard output the frequency records for a word.
* Use this for your own testing and also for query.c
*/
void print_freq_records(FreqRecord *frp) {
    int i = 0;

    while (frp != NULL && frp[i].freq != 0) {
        printf("%d    %s\n", frp[i].freq, frp[i].filename);
        i++;
    }
}

/* Given a proper directory with index and filenames file
   also given a read file descriptor and a write descriptor
   read in one word each time and run get_word on the read_word
   and write a FreqRecord in respect to the read_word   
*/
void run_worker(char *dirname, int in, int out) {
	
	// edit the dirnames to get the index and filenames 
	char file_index[PATHLENGTH];
	char file_filenames[PATHLENGTH];
	strncpy(file_index, dirname, PATHLENGTH);
	strncat(file_index, "/index", PATHLENGTH-strlen(file_index));
	file_index[PATHLENGTH-1] = '\0';
	strncpy(file_filenames, dirname, PATHLENGTH);
	strncat(file_filenames, "/filenames", PATHLENGTH-strlen(file_filenames));
	file_filenames[PATHLENGTH-1] = '\0';
	
	// build the data structure
	char **filenames = init_filenames();
	Node *head = NULL;
	read_list(file_index, file_filenames, &head, filenames);
	
	// repeatedly read the word and make it the correct format
	char word[MAXWORD];
	int read_status;
	int index;
	while ((read_status = read(in, word, MAXWORD)) > 0){
		for (int i = 0; i < strlen(word); i++){
			if (word[i] == '\n'){
				word[i] = '\0';
			}
		}
	
		// write it out
		index = 0;
		FreqRecord *fr_out = get_word(word, head, filenames);
		
		while (fr_out[index].freq > 0){
			if (write(out, &(fr_out[index]), sizeof(FreqRecord)) == -1){
				perror("write");
				exit(1);
			}
			index++;
		}
		
		free(fr_out);
		FreqRecord final;
		final.freq = 0;
		strcpy(final.filename, "");
		if (write(out, &final, sizeof(FreqRecord)) == -1){
			perror("write");
			exit(1);
		}
	}
	
	// free the memory
	for (int i = 0; i < MAXFILES; i++){
		free(filenames[i]);
	}	
	free(filenames);
	Node *to_free;
	while (head != NULL){
		to_free = head;
		head = head->next;
		free(to_free);
	}
	
	if (read_status < 0){
		perror("run_worker read");
		exit(1);
	}
	
    return;
}

/* sorted_fr is a array of FreqRecord, sorted from greatest freq to smallest
   read_fr is a FreqRecord to be added
   max_records is the max length of sorted_fr
   add read_fr to sorted_fr and remain the length and sorted property of sorted_fr
*/
void update_sort(FreqRecord *sorted_fr, FreqRecord read_fr, int max_records){
	int index = 0;
	while (read_fr.freq <= sorted_fr[index].freq){
		index++;
	}
	for (int i = max_records; i > index; i--){
		sorted_fr[i] = sorted_fr[i-1];
	}
	sorted_fr[index] = read_fr;
	sorted_fr[max_records].freq = 0;
	strcpy(sorted_fr[max_records].filename, "");
}
