#include <stdio.h>

/* Each of the n elements of array elements, is the address of an
 * array of n integers.
 * Return 0 if every integer is between 1 and n^2 and all
 * n^2 integers are unique, otherwise return 1.
 */
int check_group(int **elements, int n) {
    // TODO: replace this return statement with a real function body
	int numbers[n*n];
	int index = 0;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			numbers[n*i+j] = 0;
			for (int k = 0; k < index; k++){
				if (numbers[k] == elements[i][j] || elements[i][j] < 1 || elements[i][j] > n*n){
					return 1;
				}
			}
			numbers[index] = elements[i][j];
			index++;
		}
	}
	return 0;		
}

/* puzzle is a 9x9 sudoku, represented as a 1D array of 9 pointers
 * each of which points to a 1D array of 9 integers.
 * Return 0 if puzzle is a valid sudoku and 1 otherwise. You must
 * only use check_group to determine this by calling it on
 * each row, each column and each of the 9 inner 3x3 squares
 */
int check_regular_sudoku(int **puzzle) {
	for (int i = 0; i < 9; i++){
		//set rows
		int check_row_1[3] = {puzzle[i][0], puzzle[i][1], puzzle[i][2]};
		int check_row_2[3] = {puzzle[i][3], puzzle[i][4], puzzle[i][5]};
		int check_row_3[3] = {puzzle[i][6], puzzle[i][7], puzzle[i][8]};
		int *check_row[3] = {check_row_1, check_row_2, check_row_3};
		
		//set cols
		int check_col_1[3] = {puzzle[0][i], puzzle[1][i], puzzle[2][i]};
		int check_col_2[3] = {puzzle[3][i], puzzle[4][i], puzzle[5][i]};
		int check_col_3[3] = {puzzle[6][i], puzzle[7][i], puzzle[8][i]};
		int *check_col[3] = {check_col_1, check_col_2, check_col_3};
		
		//set boxes
		int check_box_1[3] = {puzzle[(i/3)*3][(i%3)*3], 
							  puzzle[(i/3)*3][(i%3)*3+1], 
							  puzzle[(i/3)*3][(i%3)*3+2]};
		int check_box_2[3] = {puzzle[(i/3)*3+1][(i%3)*3], 
							  puzzle[(i/3)*3+1][(i%3)*3+1], 
							  puzzle[(i/3)*3+1][(i%3)*3+2]};
		int check_box_3[3] = {puzzle[(i/3)*3+2][(i%3)*3], 
							  puzzle[(i/3)*3+2][(i%3)*3+1], 
							  puzzle[(i/3)*3+2][(i%3)*3+2]};
		int *check_box[3] = {check_box_1, check_box_2, check_box_3};
		
		//check
		if (check_group(check_col, 3) == 1 || check_group(check_row, 3) == 1 || check_group(check_box, 3) == 1 ){
			return 1;
		}
	}
	return 0;
}

