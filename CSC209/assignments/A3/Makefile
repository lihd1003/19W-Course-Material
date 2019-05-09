# Makefile for programs to index and search an index.

FLAGS = -Wall -g -std=gnu99
SRC = freq_list.c punc.c
HDR = freq_list.h worker.h
OBJ = freq_list.o punc.o

all : indexer queryone printindex query task1

indexer : indexer.o ${OBJ}
	gcc ${FLAGS} -o $@ indexer.o ${OBJ}

printindex : printindex.o ${OBJ}
	gcc ${FLAGS} -o $@ printindex.o ${OBJ}

queryone : queryone.o worker.o ${OBJ}
	gcc ${FLAGS} -o $@ queryone.o worker.o ${OBJ}
	
task1 : worker.o printindex.o ${OBJ} task1.o
	gcc ${FLAGS} -o $@ worker.o task1.o ${OBJ}
	
query : worker.o ${OBJ} query.o
	gcc ${FLAGS} -o $@ worker.o query.o ${OBJ}

# Separately compile each C file
%.o : %.c ${HDR}
	gcc ${FLAGS} -c $<

clean :
	-rm *.o indexer queryone printindex


