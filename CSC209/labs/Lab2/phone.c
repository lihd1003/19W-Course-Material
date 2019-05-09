#include <stdio.h>

int main(){
	char phone[11];
	int number;
	
	scanf("%s", &phone[0]);
	
	scanf("%d", &number);
	
	if (number == -1){
		printf("%s\n", phone);
	} else if (number >= 0 && number <= 9){
		printf("%c\n", phone[number]);
	} else {
		printf("ERROR\n");
		return 1;
	} 
	return 0;
}
