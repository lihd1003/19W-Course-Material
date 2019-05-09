/* This file is included as an example where multiple files in a single 
 * program may include the same header file.  This is usually when you
 * start seeing error messages if you put things like global variables 
 * into the header file.
 */
#include <stdio.h>
#include "ll.h"

int pop(struct node **head_p) {
    return delete_head(head_p);
}

struct node *push(struct node *head, int val) {
    return insert_head(head, val);
}
