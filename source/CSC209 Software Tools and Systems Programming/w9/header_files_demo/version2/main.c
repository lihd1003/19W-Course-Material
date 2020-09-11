#include <stdio.h>
#include "ll.c"   // This is still very wrong

void push(int);
int pop();

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
