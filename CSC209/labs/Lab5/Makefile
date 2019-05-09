# Makefile for the simulated file system program

FLAGS= -Wall -g -std=gnu99

all : bitmap_printer

bitmap_printer : bitmap_printer.o bitmap.o
	gcc ${FLAGS} -o $@ $^

bitmap_printer.o : bitmap.h
bitmap.o : bitmap.h

%.o : %.c
	gcc ${FLAGS} -c $<

clean :
	-rm *.o bitmap_printer
