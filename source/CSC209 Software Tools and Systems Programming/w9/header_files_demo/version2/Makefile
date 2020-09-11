
main : ll.o main.o stack.o
	gcc -Wall -g -std=gnu99 -o main main.o ll.o stack.o

ll.o : ll.c ll.h
	gcc -Wall -std=gnu99 -c ll.c
	
stack.o : stack.c ll.h
	gcc -Wall -std=gnu99 -c stack.c

main.o : main.c ll.h stack.h
	gcc -Wall -std=gnu99 -c main.c

clean :
	rm *.o main
