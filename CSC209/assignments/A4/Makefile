PORT=58335
CFLAGS = -DPORT=$(PORT) -g -Wall -std=gnu99
FLAGS = -g -Wall -std=gnu99
hcq_server: hcq_server.o hcq.o 
	gcc $(CFLAGS) -o hcq_server hcq_server.o hcq.o

hcq_server.o: hcq_server.c hcq.h
	gcc $(CFLAGS) -c hcq_server.c

hcq.o: hcq.c hcq.h
	gcc $(FLAGS) -c hcq.c

clean: 
	rm hcq_server *.o
