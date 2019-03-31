# portfolio
CS201 portfolio project

This program lets a user play Connect 4 against another user or against the computer. It keeps score of how many games each player wins in the continuous use of the program.

When you first run the program, it will prompt you to input a board size. You only need to enter one dimension and it will create a square board.
Next, it will ask you how many players there are (playing vs. human or computer).
Then, it will print the board and begin play. Each turn, the program will prompt you to input a column number and that is where your marker will be placed. If you pick a column that doesn't exist or that is full, the program will tell you and ask you to input a different column number. 
As soon as the winning marker is placed, the program will end the game, declare the winner, display the total score, and ask you if you want to play again.


Source Code
In my isSafe, horizDFS, vertDFS, negDiagDFS, and posDiagDFS functions, I note that they are adapted from a program on geeksforgeeks.com. The program on geeksforgeeks is for finding and counting "islands" in a 2D matrix. https://www.geeksforgeeks.org/find-number-of-islands/
I adapted its DFS recursion function and use of a "visited" matrix to form my DFS functions that I used in my main AI function.

In my takeTurn function, I note that it's adapted from a program on stackexhange. The program on stackexchange plays a connect four game between two users on a set board. https://codereview.stackexchange.com/questions/27446/connect-four-game
I borrowed this function and adapted it to work for my 2D matrix board and also when playing against the computer.
