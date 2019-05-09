FLAGS = -Wall -g -std=gnu99

starter : simplefork forkloop

all : simplefork forkloop parentcreates childcreates

simplefork : simplefork.o
	gcc ${FLAGS} -o simplefork simplefork.o

forkloop : forkloop.o
	gcc ${FLAGS} -o $@ $^

parentcreates : parentcreates.o
	gcc ${FLAGS} -o $@ $^

childcreates : childcreates.o
	gcc ${FLAGS} -o $@ $^

%.o : %.c
	gcc ${FLAGS} -c $<

clean :
	rm *.o simplefork forkloop

clean-all :
	rm *.o simplefork forkloop parentcreates childcreates
