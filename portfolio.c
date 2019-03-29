/* Rudi Johnston - rejohnston1 - 11610476
This project allows a user to play Connect Four either as a single player against AI or as two players. It keeps score between games in a continuous use of the program.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


// AI pick next move

// print board
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

// 2-player game turn-taking function
int takeTurn(char** board, int size, int player, const char *PIECES) {
	int row, col = 0;
	printf("Player %d, Enter column you want to place your marker in:	", player + 1);
	scanf("%d", &col);
	
	while (col < 1 || col > size) {
		printf("Number out of bounds! Try again.	");
		scanf("%d", &col);
	}
		
	col--;						//adjust column to 0-based matrix

	for (row = size - 1; row >= 0; row--) {		//checks column for filled spots from the bottom up
		if (board[row][col] == '_') {
			board[row][col] = PIECES[player];
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

	for (row = 0; row < size; row++) {
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
	
	for (row = 0; row < size-3; row++) {
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

// Prompt user and set board size.

	printf("Enter the game board size you would like to play on. You only need to enter one dimension as this is a square board.\nNote that if you choose to use a board of excessive size (40x40 or more), your screen may not handle it well.\n");
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

// 1-player game


// 2-player game
	if (numPlayers == 2) {
		printf("You have chosen to play against a human.\nPlayer 1's filled spots will be marked with an 'X' and Player 2's spots will be marked with an 'O'.\n");

		for (turn=0; turn < boardSize*boardSize && !done; turn++) {
			printBoard(board, boardSize);
			while(!takeTurn(board, boardSize, turn%2, PIECES)) {
				printBoard(board, boardSize);
				printf("**Column full!**\n");
			}
			done = checkWin(board, boardSize);
		}
		printBoard(board, boardSize);

		if (turn == boardSize*boardSize && !done) {
			printf("It's a tie!\n");
		}
		else {
			turn--;
			printf("Player %d wins!\n", turn%2 + 1);
		}
	}






	return 0;

}
