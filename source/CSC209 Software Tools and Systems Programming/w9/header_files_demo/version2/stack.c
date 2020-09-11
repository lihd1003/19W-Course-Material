#include <stdio.h>
#include "ll.c"

int pop() {
    return delete_head();
}

void push(int val) {
    insert_head(val);
}
