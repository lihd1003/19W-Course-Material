// Solution:

#include <stdio.h>

void lie(int *age_pt) {
    printf("You are %d years old\n", *age_pt);
    /* make sure you understand separately what 
     is going on on the right-hand side and the
     left-hand side of this assignment statement */
    *age_pt = *age_pt + 1;

    printf("You are %d years old\n", *age_pt);
}

int main() {
    int age = 18;
    lie(&age);
    printf("But your age is still %d\n", age);
    return 0; 
}
