
struct node {
    int x;
    struct node *next;
}; 

struct node *head = NULL;

void insert_head(int val);
int delete_head();
void insert_rear(int val);
int delete_rear();
void print_list();
