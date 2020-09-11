#include <stdio.h>
#include "ll.c"  // this is very very very wrong


int main() {

    insert_head(5);
    insert_head(6);
    insert_rear(10);
    print_list();

    insert_head(2);
    insert_head(8);
    delete_head();
    print_list();
    return 0;

}
