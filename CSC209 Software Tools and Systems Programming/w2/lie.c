#include <stdio.h>

void lie(int age) {
    printf("You are %d years old\n", age);
    age += 1;
    printf("You are %d years old\n", age);
}

int main() {
    int age = 18;
    lie(age);
    printf("But your age is still %d\n", age);
    return 0; 
}
