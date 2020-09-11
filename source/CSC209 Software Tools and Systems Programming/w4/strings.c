#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Part 3: Implement build_month_list.



int main() {

    // Part 1: Declare and initialize first, second, and third.


    printf("%s %s %s\n", first, second, third);


    // Part 2: Declare and initialize string_list.



    printf("%s %s %s\n", string_list[0], string_list[1], string_list[2]);


    // For part 3: Once you've implemented build_month_list, uncomment this code.
    /*
    char **months = build_month_list();
    for(int i = 0; i < 3; i++) {
        printf("%s ", months[i]);
    }
    printf("\n");

    for(int i = 0; i < 3; i++) {
        months[i][3] = '\0';
        printf("%s ", months[i]);
    }
    printf("\n");
    */

    return 0;
}
