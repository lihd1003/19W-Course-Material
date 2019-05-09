most_processes: most_processes.c 
	gcc -Wall -std=gnu99 -o most_processes most_processes.c

test_part1: most_processes
	@test_part1_output=`./most_processes  < handout.example.input`;\
	if [ ! -z "$$test_part1_output" ] && [ "$$test_part1_output" = "mcraig 5" ]; then \
		echo Compiled and sanity check passed; \
	else \
		echo Failed sanity check; \
	fi 

sudoku: sudoku.c sudoku_helpers.c
	gcc -Wall -std=gnu99 -o sudoku sudoku.c sudoku_helpers.c

test_part2: sudoku
	@test_part2_output=`./sudoku`; \
	if [ "$$test_part2_output" == "Invalid Sudoku" ]; then \
		echo Compiled and sanity check passed; \
	else \
		echo Failed sanity check; \
	fi

clean:
	rm -f *.o most_processes sudoku
