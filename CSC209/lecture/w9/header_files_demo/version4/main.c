/* See the notes in ll.c for more information about the purpose of 
 * this example.
 */
#include <stdio.h>
#include "ll.h"
#include "stack.h"


int main() {

    struct node *head = NULL;
    head = insert_head(head, 5);
    head = insert_head(head, 6);
    head = insert_rear(head, 10);
    print_list(head);

    head = insert_head(head, 2);
    head = insert_head(head, 8);
    print_list(head);
    int y = delete_head(&head);
    printf("value deleted from head was %d\n", y);

    print_list(head);

    struct node *stack = NULL;
    stack = push(stack, 15);
    int val = pop(&stack);
    printf("popped value is %d\n", val);
    print_list(head);
    return 0;

}
