/* The functions operate on a linked list of words.  Each element of the
* list contains a word, and an array that stores the frequency of the
* word for each file in a particular list of files.  The name of each file 
* in that list is stored in an array of file names.  The array in a
* linked list node is a parallel array to the array of file names.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freq_list.h"

int num_words = 0;

/* Allocate and initialize a new node for the list.
*/
Node *create_node(char *word, int count, int filenum) {
    Node *newnode;
    if ((newnode = malloc(sizeof(Node))) == NULL) {
        perror("create_node");
        exit(1);
    }

    strncpy(newnode->word, word, MAXWORD);
    /* Make sure it is null terminated. */
    newnode->word[MAXWORD - 1] = '\0';

    memset(newnode->freq, 0, MAXFILES * sizeof(int));
    newnode->freq[filenum] = count;
    newnode->next = NULL;
    return newnode;
}

/* Increment the frequency of "word" for the file "fname" in the list
* pointed to by "head".  If the word is in the list, uses the filenames
* array to determine which element of the freq array for that word
* should be incremented. If the word is not in the list, add it in
* alphabetical order and set the frequency of the word in the file
* fname to 1.
*/
Node *add_word(Node *head, char **filenames, char *word, char *fname) {
    int filenum;
    Node *cur = head;
    Node *prev = head;

    filenum = get_filenum(fname, filenames);
    if (cur && (strcmp(cur->word, word)) > 0) {
        head = create_node(word, 1, filenum);
        head->next = cur;
        num_words++;
        return head;
    }

    /* look for the word */
    while (cur != NULL) {
        if ((strcmp(cur->word, word)) == 0) {
            /* found word */
            cur->freq[filenum] += 1;
            return head;
        } else if ((strcmp(cur->word, word)) > 0) {
            /* need to insert word */
            if (cur == prev ) { /* we are at the head */
                prev = create_node(word, 1, filenum);
                prev->next = cur;
                num_words++;
                return prev;
            } else {
                prev->next = create_node(word, 1, filenum);
                prev->next->next = cur;
                num_words++;
                return head;
            }
        }

        prev = cur;
        cur = cur->next;
    }

    /* this word was not already in the list */
    if (cur == prev) {
        num_words++;
        return create_node(word, 1, filenum);
    } else {
        if (cur == NULL) {
            num_words++;
            prev->next = create_node(word, 1, filenum);
        }
        return head;
    }
}

/* Print the list to standard output in a readable format. 
* (Primarily useful for debugging purposes.)
*/
void display_list(Node *head, char **filenames) {
    int i;

    while (head != NULL) {
        printf("%s\n", head->word);

        for (i = 0; i < MAXFILES; i++) {
            if (filenames[i] != NULL) {
                printf("    %d %s ", head->freq[i], filenames[i]);
            } else {
                printf("\n");
                break;
            }
        }
        head = head->next;
    }
}

/* Print the linked list of words to two files.  The array of file names
* will be written one line per file in text format to namefile.  The
* linked list will be written to the file listfile in binary format.
*/
void write_list(char *namefile, char *listfile, Node *head, char **filenames) {
    Node *cur = head;
    int i;

    /* Write out the linked list */
    FILE *list_fp;
    if ((list_fp = fopen(listfile, "w")) == NULL) {
        perror("fopen for list file");
        exit(1);
    }

    while (cur != NULL) {
        if (fwrite(cur, sizeof(Node), 1, list_fp) != 1) {
            perror("fwrite for list file");
            exit(1);
        }
        cur = cur->next;
    }

    if (fclose(list_fp)) {
        perror("fclose for list file");
    }

    /* Write the file names array */
    FILE *fname_fp;
    if ((fname_fp = fopen(namefile, "w")) == NULL) {
        perror("fopen for names file");
        exit(1);
    }

    for (i = 0; i < MAXFILES && filenames[i] != NULL; i++) {
        fprintf(fname_fp, "%s\n", filenames[i]);
    }

    if (fclose(fname_fp)) {
        perror("fclose for names file");
    }
}

/* Populate the linked list and filenames data structures with data
* stored in two files.  The data in namefile is used to construct the
* filenames array, and the data in listfile is used to construct a
* linked list.  Note that filenames must point to an array of the
* correct size, but that head does not point to a list node when it is
* passed in.
*/
void read_list(char *listfile, char *namefile, 
                    Node **head, char **filenames) {

    /* Read in the linked list */
    FILE *list_fp;
    if ((list_fp = fopen(listfile, "r")) == NULL) {
        perror("fopen for list_fp");
        exit(1);
    }

    Node *cur = malloc(sizeof(Node));
    if (cur == NULL) {
        perror("malloc for current node");
        exit(1);
    }

    Node *prev = NULL;
    *head = cur;

    while ((fread(cur, sizeof(Node), 1, list_fp)) != 0) {
        cur->next = NULL;

        if (cur == *head) {
            prev = cur;
        } else {
            prev->next = cur;
            prev = cur;
        }

        cur = malloc(sizeof(Node));
        if (cur == NULL) {
            perror("malloc for current node");
            exit(1);
        }
    }

    /* Check if the file is empty. */
    if (*head == cur)
        *head = NULL;

    /* Release memory not being used. */
    free(cur);

    if ((fclose(list_fp))) {
        perror("fclose for list_fp");
    }

    /* Read in the file names */
    FILE *fname_fp;
    if ((fname_fp = fopen(namefile, "r")) == NULL) {
        perror("fopen for fname_fp");
        exit(1);
    }

    char line[MAXLINE];
    int i = 0;
    while ((fgets(line, MAXLINE, fname_fp)) != NULL) {
        line[strlen(line) - 1] = '\0';

        char *name = malloc(strlen(line) + 1);
        if (name == NULL) {
            perror("malloc for name");
            exit(1);
        }

        strncpy(name, line, (strlen(line) + 1));
        filenames[i] = name;
        i++;

        if (i == MAXFILES) {
            fprintf(stderr, "Invalid input file! Too many filenames!\n");
            exit(1);
        }
    }

    if ((fclose(fname_fp))) {
        perror("fclose for fname_fp");
    }
}

/* Create an array to hold filenames and initialize it to all NULL 
*/
char **init_filenames() {
    int i;
    char **fnames;

    if ((fnames = malloc(MAXFILES * sizeof(char *))) == NULL) {
        perror("malloc for init_filenames");
        exit(1);
    }

    for (i = 0; i < MAXFILES; i++) {
        fnames[i] = NULL;
    }
    return fnames;
}

/* If fname is in the filenames array, then return its index.
* Otherwise add the filename to the array and return the new index.
* Currently implemented as a linear search.
*/
int get_filenum(char *fname, char **filenames) {
    int i;

    for (i = 0; i < MAXFILES; i++) {
        if (filenames[i] == NULL) {
            filenames[i] = malloc(strlen(fname) + 1);
            if (filenames[i] == NULL) {
                perror("malloc for get_filenum");
                exit(1);
            }
            strncpy(filenames[i], fname, strlen(fname) + 1);
            return i;
        }

        if ((strcmp(fname, filenames[i])) == 0) {
            return i;
        }
    }

    for (i = 0; i < MAXFILES; i++) {
        if (filenames[i] != NULL) {
            fprintf(stderr, "filenames[%d] = %s\n", i, filenames[i]);
        } else {
            break;
        }
    }

    fprintf(stderr, "Too many files\n");
    exit(1);
}
