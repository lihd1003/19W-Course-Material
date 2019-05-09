#include <stdio.h>

#define SIZE 4
#define OVERFLOW 4

int main() {
    int index = 0;
    int i;
    int before[SIZE] = {10, 10, 10, 10};
    int a[SIZE] = {0, 0, 0, 0};
    int after[SIZE] = {10, 10, 10, 10};

    printf("Address of the variables:\n");
    for (index = 0; index < SIZE; index++) {
        printf("%lx -> &after[%d]\n", (unsigned long) &after[index], index);
    }
    for (index = 0; index < SIZE; index++) {
        printf("%lx -> &a[%d]\n", (unsigned long) &a[index], index);
    }
    for (index = 0; index < SIZE; index++) {
        printf("%lx -> &before[%d]\n", (unsigned long) &before[index], index);
    }
    printf("%lx -> &i\n", (unsigned long)&i);
    printf("%lx -> &index\n", (unsigned long)&index);
    printf("\n");


    printf("Initial values:\n");
    printf("i = %d\n", i);
    printf("before = {%d, %d, %d, %d}\n", before[0], before[1], before[2], before[3]);
    printf("a = {%d, %d, %d, %d}\n", a[0], a[1], a[2], a[3]);
    printf("after = {%d, %d, %d, %d}\n", after[0], after[1], after[2], after[3]);
    printf("\n");


    for (i = 0; i < OVERFLOW; i++) {
        a[i] = i * 10;
        printf("i = %d\n", i);
        printf("before = {%d, %d, %d, %d}\n", before[0], before[1], before[2], before[3]);
        printf("a = {%d, %d, %d, %d}\n", a[0], a[1], a[2], a[3]);
        printf("after = {%d, %d, %d, %d}\n", after[0], after[1], after[2], after[3]);
    }

    return 0;
}
