#include <stdio.h>
#include "ll.h"
#include "stack.h"

struct node *head = NULL;

int main() {

    insert_head(5);
    insert_head(6);
    insert_rear(10);
    print_list();
    push(20);

    insert_head(2);
    insert_head(8);
    pop();

    print_list();
    return 0;

}
