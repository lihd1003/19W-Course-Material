#include <stdio.h>

int main(){
	char phone[11];
	int number;
	int error_condition = 0;
	scanf("%s", &phone[0]);
	while (scanf("%d", &number) != EOF){
		if (number == -1){
			printf("%s\n", phone);
			number = 0;
		} else if (number > -1 && number <= 9) {
			printf("%c\n", phone[number]);
		} else {
			printf("ERROR\n");
			error_condition = 1;
		}
	}
	return error_condition;
}
