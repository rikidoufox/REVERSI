//
// Author:
//
#include "reversi.h"
#define MAX_BOARD_SIZE 26

void printBoard(char board[][MAX_BOARD_SIZE], int n) {
    printf("  ");
    for (int i = 0; i < n; i++) {
        printf("%c", 'a' + i);
    }
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("%c ", 'a' + i);
        for (int j = 0; j < n; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
}

bool positionInBounds(int n, int row, int col) {
    return row >= 0 && row < n && col >= 0 && col < n;
}

bool checkLegalInDirection(char board[][MAX_BOARD_SIZE], int n, int row, int col, char colour, int deltaRow, int deltaCol) {
    char opponentColour = (colour == 'W') ? 'B' : 'W';
    int newRow = row + deltaRow;
    int newCol = col + deltaCol;

    if (!positionInBounds(n, newRow, newCol) || board[newRow][newCol] != opponentColour) {
        return false;
    }

    while (positionInBounds(n, newRow, newCol)) {
        newRow += deltaRow;
        newCol += deltaCol;

        if (!positionInBounds(n, newRow, newCol)) {
            return false;
        }

        if (board[newRow][newCol] == 'U') {
            return false;
        }

        if (board[newRow][newCol] == colour) {
            return true;
        }
    }

    return false;
}

bool isValidMove(char board[][MAX_BOARD_SIZE], int n, int row, int col, char colour) {
    if (board[row][col] != 'U') {
        return false;
    }

    for (int dRow = -1; dRow <= 1; dRow++) {
        for (int dCol = -1; dCol <= 1; dCol++) {
            if (dRow == 0 && dCol == 0) {
                continue;
            }

            if (checkLegalInDirection(board, n, row, col, colour, dRow, dCol)) {
                return true;
            }
        }
    }

    return false;
}

void makeMove(char board[][MAX_BOARD_SIZE], int n, int row, int col, char colour) {
    board[row][col] = colour;

    for (int dRow = -1; dRow <= 1; dRow++) {
        for (int dCol = -1; dCol <= 1; dCol++) {
            if (dRow == 0 && dCol == 0) {
                continue;
            }

            if (checkLegalInDirection(board, n, row, col, colour, dRow, dCol)) {
                int newRow = row + dRow;
                int newCol = col + dCol;

                while (board[newRow][newCol] != colour) {
                    board[newRow][newCol] = colour;
                    newRow += dRow;
                    newCol += dCol;
                }
            }
        }
    }
}

void computerMove(char board[][MAX_BOARD_SIZE], int n, char playerColour) {
    char computerColour = (playerColour == 'W') ? 'B' : 'W'; // Determine computer's color

    int bestRow = -1;
    int bestCol = -1;
    int maxFlips = -1;
    int blankCount = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'U') {
                blankCount++;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'U' && isValidMove(board, n, i, j, computerColour)) {
                int flips = 0;

                for (int dRow = -1; dRow <= 1; dRow++) {
                    for (int dCol = -1; dCol <= 1; dCol++) {
                        if (dRow == 0 && dCol == 0) {
                            continue;
                        }

                        if (checkLegalInDirection(board, n, i, j, computerColour, dRow, dCol)) {
                            int newRow = i + dRow;
                            int newCol = j + dCol;

                            while (board[newRow][newCol] != computerColour) {
                                flips++;
                                newRow += dRow;
                                newCol += dCol;
                            }
                        }
                    }
                }

                if (flips > maxFlips) {
                    maxFlips = flips;
                    bestRow = i;
                    bestCol = j;
                }
            }
        }
    }

    if (bestRow != -1 && bestCol != -1) {
        makeMove(board, n, bestRow, bestCol, computerColour);
        printf("Computer places %c at %c%c.\n", computerColour, 'a' + bestRow, 'a' + bestCol);
        printBoard(board, n);
    }
    else{
        if(blankCount!=0){
            printf("%c player has no valid move.\n", computerColour);
        }
   }
}

int main(void) {
    char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
    int n;
    char row, col, playerColour;

    printf("Enter the board dimension: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = 'U';
        }
    }

    printf("Computer plays (B/W): ");
    scanf(" %c", &playerColour);

    char computerColour = (playerColour == 'W') ? 'B' : 'W'; // Determine computer's color

    board[n / 2 - 1][n / 2 - 1] = 'W';
    board[n / 2][n / 2] = 'W';
    board[n / 2 - 1][n / 2] = 'B';
    board[n / 2][n / 2 - 1] = 'B';

    printBoard(board, n);
    if (playerColour == 'B') {
        computerMove(board, n, computerColour);
    }
    int check=0;
    while (true) {
        printf("Enter move for colour %c (RowCol): ", computerColour);
        scanf(" %c%c", &row, &col);

        int rowIndex = row - 'a';
        int colIndex = col - 'a';

        if (!positionInBounds(n, rowIndex, colIndex) || !isValidMove(board, n, rowIndex, colIndex, computerColour)){
            printf("Invalid move.\n");
            printf("%c player wins.\n", playerColour);
            return 0;
        } else {
            makeMove(board, n, rowIndex, colIndex, computerColour);
            printBoard(board, n);
            computerMove(board, n, computerColour);
        }

        int blankCount = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'U') {
                    blankCount++;
                }
            }
        }

        if (blankCount == 0) {
            break;
        }

        bool validMoveExists = false;
        for (int i = 0; i < n && !validMoveExists; i++) {
            for (int j = 0; j < n && !validMoveExists; j++) {
                if (board[i][j] == 'U' && isValidMove(board, n, i, j, computerColour)){
                    validMoveExists = true;
                }
            }
        }

        if (!validMoveExists) {
            printf("%c player has no valid move.\n", computerColour);
            computerMove(board, n, computerColour);
        }
    }

    int whiteCount = 0;
    int blackCount = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'W') {
                whiteCount++;
            } else if (board[i][j] == 'B') {
                blackCount++;
            }
        }
    }

    if (whiteCount > blackCount) {
        printf("W player wins.\n");
    } else if (blackCount > whiteCount) {
        printf("B player wins.\n");
    } else {
        printf("It's a tie.\n");
    }

    return 0;
}

