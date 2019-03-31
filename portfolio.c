/* Rudi Johnston - rejohnston1 - 11610476
This project allows a user to play Connect Four either as a single player against AI or as two players. It keeps score between games in a continuous use of the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


// function that checks if a given cell can be included in the DFS
// adapted from geeksforgeeks, see README
int isSafe(char** board, int size, int row, int col, bool** visited) {
	return (row>=0) && (row<size) && (col>=0) && (col<size) && (board[row][col] == 'O') && (!visited[row][col]); 
}

// function that does a DFS on a 2D boolean matrix, counts the two horizonal neighbors as adjacent vertices
// adapted from geeksforgeeks, see README
void horizDFS(char** board, int size, int *hCount, int *hIndex, int row, int col, bool** visited) {
	int rowNum[] = {0, 0};					// arrays used to get coordinates of horizontal neighbors
	int colNum[] = {-1, 1};

	visited[row][col] = true;				// marks current cell as visited

	for (int k=0; k<2; k++) {
		if (isSafe(board, size, row+rowNum[k], col+colNum[k], visited)) {
			hCount++;				
			if (k==0)				// increasing count in-a-row and moving index of last spot
				hIndex--;
			else if (k==1)
				hIndex++;
			horizDFS(board, size, hCount, hIndex, row+rowNum[k], col+colNum[k], visited);
		}
	}

return;
}

// function that does a DFS on a 2D boolean matrix, counts the two vertical neighbors as adjacent vertices
// adapted from geeksforgeeks, see README
void vertDFS(char** board, int size, int *vCount, int *vIndex, int row, int col, bool** visited) {
	int rowNum[] = {-1, 1};					
	int colNum[] = {0, 0};

	visited[row][col] = true;				

	for (int k=0; k<2; k++) {
		if (isSafe(board, size, row+rowNum[k], col+colNum[k], visited)) {
			vCount++;
			vIndex = &col;
			vertDFS(board, size, vCount, vIndex, row+rowNum[k], col+colNum[k], visited);
		}
	}
return;
}

// function that does a DFS on a 2D boolean matrix, counts the two diagonal neighbors that make a negative slope as adj. vertices
// adapted from geeksforgeeks, see README
void negDiagDFS(char** board, int size, int *negDCount, int *negDIndex, int row, int col, bool** visited) {
	int rowNum[] = {-1, 1};
	int colNum[] = {-1, 1};

	visited[row][col] = true;

	for (int k=0; k<2; k++) {
		if (isSafe(board, size, row+rowNum[k], col+colNum[k], visited)) {
			negDCount++;
			if (k==0)
				negDIndex--;
			else if (k==1)
				negDIndex++;
			negDiagDFS(board, size, negDCount, negDIndex, row+rowNum[k], col+colNum[k], visited);
		}
	}
return;
}

// function that does a DFS on a 2D boolean matrix, counts the two diagonal neighbors that make a positive slope as adj. vertices
// adapted from geeksforgeeks, see README
void posDiagDFS(char** board, int size, int *posDCount, int *posDIndex, int row, int col, bool** visited) {
	int rowNum[] = {1, -1};
	int colNum[] = {-1, 1};

	visited[row][col] = true;

	for (int k=0; k<2; k++) {
		if (isSafe(board, size, row+rowNum[k], col+colNum[k], visited)) {
			posDCount++;
			if (k==0)
				posDIndex--;
			else if (k==1)
				posDIndex++;
			posDiagDFS(board, size, posDCount, posDIndex, row+rowNum[k], col+colNum[k], visited);
		}
	}
return;
}

// AI pick next move
void computerTurn(char** board, int size, bool** visited) {
	int hCount = 0; int vCount = 0; int negDCount = 0; int posDCount = 0;
	int *hCountTemp = (int *)1;
	int *vCountTemp = (int *)1;
	int *negDCountTemp = (int *)1;
	int *posDCountTemp = (int *)1;
	int hIndex = 0; int vIndex = 0; int negDIndex = 0; int posDIndex = 0;
	int *hIndexTemp = (int *)0;
	int *vIndexTemp = (int *)0;
	int *negDIndexTemp = (int *)0;
	int *posDIndexTemp = (int *)0;

	// use DFS functions to find largest current streak and add next marker to next column/row
	for (int i=size-1; i>=0; i--) {
		for (int j=0; j<size; j++) {
			if (board[i][j] == 'O') {
				hIndexTemp = &j;
				vIndexTemp = &j;
				negDIndexTemp = &j;
				posDIndexTemp = &j;

				horizDFS(board, size, hCountTemp, hIndexTemp, i, j, visited);
				if (hCountTemp > &hCount) {
					hCount = *hCountTemp;
					hIndex = *hIndexTemp;
				}
			
				vertDFS(board, size, vCountTemp, vIndexTemp, i, j, visited);
				if (vCountTemp > &vCount) {
					vCount = *vCountTemp;
					vIndex = *vIndexTemp;
				}

				negDiagDFS(board, size, negDCountTemp, negDIndexTemp, i, j, visited);
				if (negDCountTemp > &negDCount) {
					negDCount = *negDCountTemp;
					negDIndex = *negDIndexTemp;
				}

				posDiagDFS(board, size, posDCountTemp, posDIndexTemp, i, j, visited);
				if (posDCountTemp > &posDCount) {
					posDCount = *posDCountTemp;
					posDIndex = *posDIndexTemp;
				}
			}
		}
	}
	
	if ((hCount >= vCount) && (hCount >= negDCount) && (hCount >= posDCount)) {		// place horizontal move
		for (int a=size-1; a>=0; a--) {
			if ((hIndex != size-1) && (board[a][hIndex+1] == '_')) {
				board[a][hIndex+1] = 'O';
				return;
			}
			else if ((hIndex != 0) && (board[a][hIndex-1] == '_')) {
				board[a][hIndex-1] = 'O';
				return;
			}
		}
	}

	else if ((vCount >= hCount) && (vCount >= negDCount) && (vCount >= posDCount)) {		// place vertical move
		for (int a=size-1; a>=0; a--) {
			if (board[a][vIndex] == '_') {
				board[a][vIndex] = 'O';
				return;
			}
		}
	}

	else if ((negDCount >= hCount) && (negDCount >= vCount) && (negDCount >= posDCount)) {		// place neg. slope diag. move
		for (int a=size-1; a>0; a--) {
			if ((negDIndex != 0) && (board[a-1][negDIndex-1] == '_') && (board[a][negDIndex-1] != '_')) {
				board[a-1][negDIndex-1] = 'O';
				return;
			}
		}
		for (int a=size-2; a>=0; a--) {
			if ((negDIndex != size-1) && (board[a+1][negDIndex+1] == '_') && (board[a+2][negDIndex+1] != '_')) {
				board[a+1][negDIndex+1] = 'O';
				return;
			}
		}
	}
	
	else if ((posDCount >= hCount) && (posDCount >= vCount) && (posDCount >= negDCount)) {		// place pos. slope diag. move
		for (int a=size-2; a>=0; a--) {
			if ((posDIndex != 0) && (board[a+1][posDIndex-1] == '_') && (board[a+2][posDIndex-1] != '_')) {
				board[a+1][posDIndex-1] = 'O';
				return;
			}
		}
		for (int a=size-1; a>0; a--) {
			if ((posDIndex != size-1) && (board[a-1][posDIndex+1] == '_') && (board[a][posDIndex+1] != '_')) {
				board[a-1][posDIndex+1] = 'O';
				return;
			}
		}
	}
		
	// if none of the advantageous spots were open, place in first open spot found
	for (int a=size-1; a>=0; a--) {
		for (int b=0; b<size; b++) {
			if (board[a][b] == '_') {
				board[a][b] = 'O';
				return;
			}
		}
	}
}

// function that prints the board
void printBoard(char** board, int size) {
	for (int i=0; i<size; i++) {
		for (int j=0; j<size; j++) {
			printf(" %c", board[i][j]);
			if (j==(size-1)) {
				printf("\n");
			}
		}
	}

return;
}

// function that prompts user to take their turn and inserts markers into their chosen column
// adapted from stackexchange, see README
int takeTurn(char** board, int size, int player, const char *PIECES) {
	int row, col = 0;
	printf("\nPlayer %d, Enter the column you want to place your marker in:     ", player + 1);
	scanf("%d", &col);
	
	while (col < 1 || col > size) {
		printf("Number out of bounds. Try again.	");
		scanf("%d", &col);
	}
		
	col--;								//adjust column number to 0-based matrix

	for (row = size - 1; row >= 0; row--) {				//checks column for filled spots from the bottom up
		if (board[row][col] == '_') {
			board[row][col] = PIECES[player];		// inserts player's piece in column
			return 1;
		}
	}
	return 0;
} 

// function to check for four in a row
int checkFour(char** board, int a, int aa, int b, int bb, int c, int cc, int d, int dd) {
	return (board[a][aa] == board[b][bb] && board[b][bb] == board[c][cc] && board[c][cc] == board[d][dd] && board[a][aa] != '_');
}

// function to check for four in a row horizontally
int checkHoriz(char** board, int size) {
	int row, col;
	for (row = size-1; row >= 0; row--) {
		for (col = 0; col < size-3; col++) {
			if (checkFour(board, row, col, row, col+1, row, col+2, row, col+3)) {
				return 1;
			}
		}
	}
	
	return 0;
}

// function to check for four in a row vertically
int checkVert(char** board, int size) {
	int row, col;
	for (row = size-4; row >= 0; row--) {
		for (col = 0; col < size; col++) {
			if (checkFour(board, row, col, row+1, col, row+2, col, row+3, col)) {
				return 1;
			}
		}
	}

	return 0;
}

// function to check for four in a row diagonally
int checkDiag(char** board, int size) {
	int row, col;
	for (row = 0; row < size-3; row++) {
		for (col = 0; col < size-3; col++) {
			if (checkFour(board, row, col, row+1, col+1, row+2, col+2, row+3, col+3)) {
				return 1;
			}
		}
	}

	for (row = size-1; row >= 3; row--) {
		for (col = 0; col < size-3; col++) {
			if (checkFour(board, row, col, row-1, col+1, row-2, col+2, row-3, col+3)) {
				return 1;
			}
		}
	}	

	return 0;
}

// function to check if the game has been won
int checkWin(char** board, int size) {
	return (checkHoriz(board, size) || checkVert(board, size) || checkDiag(board, size));
}



// main program
int main(void) {
	
	int turn = 0;
	int boardSize;
	bool done = false;
	int playerOneScore = 0;
	int playerTwoScore = 0;
	bool playAgain = true;

// Prompt user for board size and set board size.

	printf("Enter the game board size you would like to play on. You only need to enter one dimension, this is a square board.\n");
	printf("Note that if you choose to use a board of excessive size (40x40 or more), your screen may not handle it well.\n");
	scanf("%d", &boardSize);

	while (boardSize < 4) {
		printf("You must enter a board size of 4x4 or greater. Please enter the game board size you would like to play on.\n");
		scanf("%d", &boardSize);
	}

// Create board and initialize with blanks.
	const char *PIECES = "XO";
	char **board;
	board = malloc(boardSize * sizeof *board);
	for (int a=0; a<boardSize; a++) {
		board[a] = malloc(boardSize * sizeof *board[a]);
	}	

	for (int i=0; i<boardSize; i++) {
		for (int j=0; j<boardSize; j++) {
			board[i][j] = '_';
		}
	}

// Prompt if game is 1-player or 2-player
	int numPlayers;
	printf("How many players? 1 (human vs. computer) or 2 (human vs. human)\n");
	scanf("%d", &numPlayers);
	while ((numPlayers != 1) && (numPlayers != 2)) {
		printf("Please enter a 1 or 2 to choose number of players.\n");
		scanf("%d", &numPlayers);
	}

	while (playAgain == true) {
	// 1-player game
		if (numPlayers == 1) {
			printf("You have chosen to play against the computer.\n");
			printf("Your filled spots will be marked with an 'X' and the computer's spots will be marked with an 'O'.\n");

		// create matrix to mark visited cells
			bool **visited;
			visited = malloc(boardSize * sizeof *visited);
			for (int a=0; a<boardSize; a++) {
				visited[a] = malloc(boardSize * sizeof *visited[a]);
			}

		// actual play
			for (turn=0; turn < boardSize*boardSize && !done; turn++) {
				printBoard(board, boardSize);
	
				if (turn%2 == 0) {						// human turn
					while(!takeTurn(board, boardSize, 0, PIECES)) {
						printBoard(board, boardSize);
						printf("Column full. Please try a different column.	");
					}
					done = checkWin(board, boardSize);
				}

				else if (turn%2 == 1) {						// computer turn
					// every turn: reset visited matrix, then call AI turn function, then check if done

					printf("\nComputer's turn: \n");		

					for (int i=0; i<boardSize; i++) {
						for (int j=0; j<boardSize; j++) {
							visited[i][j] = false;
						}
					}
							
					if (turn == 1) {
						if (board[boardSize-1][0] == '_')
							board[boardSize-1][0] = 'O';
						else
							board[boardSize-1][1] = 'O';
					}					
		
					else {
						computerTurn(board, boardSize, visited);
					}
			
					done = checkWin(board, boardSize);
				}
			}
			printBoard(board, boardSize);
	
			if (turn == boardSize*boardSize && !done) {
				printf("It's a tie!\n");
			}
			else {
				if (turn%2 == 1) {
					printf("\nYou win!\n");
					playerOneScore++;
				}
				else {
					printf("\nThe computer wins!\n");
					playerTwoScore++;
				}
			}
			
			printf("\nScore --- You: %d        Computer: %d \n", playerOneScore, playerTwoScore);
		}

	// 2-player game
		if (numPlayers == 2) {
			printf("You have chosen to play against a human.\n");
			printf("Player 1's spots will be marked with an 'X' and Player 2's spots will be marked with an 'O'.\n");

			for (turn=0; turn < boardSize*boardSize && !done; turn++) {
				printBoard(board, boardSize);
				while(!takeTurn(board, boardSize, turn%2, PIECES)) {		// takeTurn returns 0 if column is full
					printBoard(board, boardSize);
					printf("Column full. Please try a different column.	");
				}
				done = checkWin(board, boardSize);
			}
			printBoard(board, boardSize);

			if (turn == boardSize*boardSize && !done) {
				printf("It's a tie!\n");
			}
			else {
				turn--;
				printf("\nPlayer %d wins!\n", turn%2 + 1);
				
				if (turn%2 == 0)
					playerOneScore++;
				else if (turn%2 == 1)
					playerTwoScore++;
			}

			printf("\nScore --- Player 1: %d        Player 2: %d \n", playerOneScore, playerTwoScore);
		}
	
		for (int i=0; i<boardSize; i++) {					// reset the board
			for (int j=0; j<boardSize; j++) {
				board[i][j] = '_';
			}
		}
	
		printf("\nPlay again? Yes(Y) or No(N):	");
		char yesNo;
		scanf("%s", &yesNo);
		while ((yesNo != 'Y') && (yesNo != 'N') && (yesNo != 'y') && (yesNo != 'n')) {
			printf("\nPlease input a 'Y' or 'N' to choose whether you want to play again:	");
			scanf("%s", &yesNo);
		}

		if ((yesNo == 'Y') || (yesNo == 'y')) {
			playAgain = true;
		}
		else if ((yesNo == 'N') || (yesNo == 'n')) {
			playAgain = false;
		}
		
	}


return 0;

}
