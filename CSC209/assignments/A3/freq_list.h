#ifndef FREQ_LIST_H
#define FREQ_LIST_H

#define MAXFILES 50
#define MAXWORD 32
#define MAXLINE 1024
#define PATHLENGTH 128

struct node {
    char word[MAXWORD];
    int freq[MAXFILES];
    struct node *next;
};

typedef struct node Node; 

extern char *filenames[MAXFILES];
extern int num_words;

Node *create_node(char *word, int count, int filenum);
Node *add_word(Node *head, char **filenames, char *word, char *fname);
void print_list(FILE *fp, struct node *head);
char **init_filenames();
int get_filenum(char *fname, char **filenames);
void display_list(Node *head, char **filenames);
void write_list(char *namefile, char *listfile, Node *head, char **filenames);
void read_list(char *namefile, char *listfile, Node **head, char **filenames);

#endif /* FREQ_LIST_H */
