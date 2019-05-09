#include <stdio.h>
#include <string.h>

int main(int argc, char**argv){
	
	// inputs
	char in_str[1024];
	
	// reading 
	char read_uid[32];
	char read_ppid[32];
	
	// max
	char max_uid[32];
	int max_count = 0;
	
	// current
	char cur_uid[32];
	int cur_count = 0;

	// no argument given
	if (argc == 1){
		if (fgets(in_str, 1024, stdin) != NULL){
			sscanf(in_str, "%[^ ]", cur_uid);
			cur_count += 1;
		}
		while (fgets(in_str, 1024, stdin) != NULL){
			sscanf(in_str, "%[^ ]", read_uid);
			
			if (strcmp(read_uid, cur_uid) == 0){
				cur_count += 1;
			} else if (cur_count > max_count){
				max_count = cur_count;
				strcpy(max_uid, cur_uid);
				cur_count = 1;
				strcpy(cur_uid, read_uid);
			} else {
				cur_count = 1;
				strcpy(cur_uid, read_uid);
			}
			
		}
		if (cur_count > max_count){
			max_count = cur_count;
			strcpy(max_uid, cur_uid);
		}
	} 
	// given ppid
	else if (argc == 2){
		char target_ppid[32];
		strcpy(target_ppid, argv[1]);
		
		if (fgets(in_str, 1024, stdin) != NULL){
			sscanf(in_str, "%[^ ]%*[ ]%*[^ ]%*[ ]%[^ ]", read_uid, read_ppid);
			if (strcmp(read_ppid, target_ppid) == 0){
				cur_count = 1;
				strcpy(cur_uid, read_ppid);
			}
		} 
		while (fgets(in_str, 1024, stdin) != NULL){
			sscanf(in_str, "%[^ ]%*[ ]%*[^ ]%*[ ]%[^ ]", read_uid, read_ppid);
			if (strcmp(read_uid, cur_uid) == 0 && strcmp(read_ppid, target_ppid) == 0){
				cur_count += 1;
			} else if (strcmp(read_ppid, target_ppid) == 0){
				if (cur_count > max_count){
					strcpy(max_uid, cur_uid);
					max_count = cur_count;
				}
				strcpy(cur_uid, read_uid);
				cur_count = 1;
			}
			
		}
		if (cur_count > max_count){
			max_count = cur_count;
			strcpy(max_uid, cur_uid);
		}
	}
	// given incorrect number of arguments
	else {
		fprintf(stderr, "USAGE: most_processes [ppid]\n");
		return 1;
	}
	if (max_count > 0){
		printf("%s %d\n", max_uid, max_count);
	}
	return 0;
}
