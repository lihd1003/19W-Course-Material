// Solution:

#include <stdio.h>

float change(int *b, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        b[i] = b[i] + 10;
        sum += b[i];
    }
    return (float) sum / size;
}

int main() {
    int a[4] = {10, 20, 30, 40};
    float result = change(a, 4);
    return 0;
}
