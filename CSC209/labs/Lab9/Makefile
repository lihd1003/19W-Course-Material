FLAGS = -Wall -g -std=gnu99

all : time_reads write_test_file

time_reads : time_reads.c
	gcc ${FLAGS} -o $@ $^

write_test_file : write_test_file.c
	gcc ${FLAGS} -o $@ $^

output : write_test_file
	./write_test_file output

clean :
	rm -f time_reads write_test_file output
