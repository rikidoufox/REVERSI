#include "lab8part2.h"
#include "liblab8part2.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define MAX_BOARD_SIZE 26

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

int minimax(char board[][MAX_BOARD_SIZE], int n, char turn, int depth, bool maximizingPlayer);

int makeMove(const char board[][MAX_BOARD_SIZE], int n, char turn, int *row, int *col) {
    int bestScore = -1000;
    *row = -1;
    *col = -1;

    char newBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'U' && isValidMove(board, n, i, j, turn)) {
                for (int x = 0; x < n; x++) {
                    for (int y = 0; y < n; y++) {
                        newBoard[x][y] = board[x][y];
                    }
                }
                newBoard[i][j] = turn;

                int score = minimax(newBoard, n, (turn == 'W') ? 'B' : 'W', 4, false);
                if (score > bestScore) {
                    bestScore = score;
                    *row = i;
                    *col = j;
                }
            }
        }
    }

    return (bestScore != -1000) ? 1 : 0; // Indicate success if a move is found
}
int minimax(char board[][MAX_BOARD_SIZE], int n, char turn, int depth, bool maximizingPlayer) {
    if (depth == 0) {
        return evaluateBoard(board, n);
    }

    if (maximizingPlayer) {
        int bestScore = -1000;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'U' && isValidMove(board, n, i, j, turn)) {
                    char newBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
                    for (int x = 0; x < n; x++) {
                        for (int y = 0; y < n; y++) {
                            newBoard[x][y] = board[x][y];
                        }
                    }
                    newBoard[i][j] = turn;

                    int score = minimax(newBoard, n, (turn == 'W') ? 'B' : 'W', depth - 1, false);
                    if (score > bestScore) {
                        bestScore = score;
                    }
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == 'U' && isValidMove(board, n, i, j, turn)) {
                    char newBoard[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
                    for (int x = 0; x < n; x++) {
                        for (int y = 0; y < n; y++) {
                            newBoard[x][y] = board[x][y];
                        }
                    }
                    newBoard[i][j] = turn;

                    int score = minimax(newBoard, n, (turn == 'W') ? 'B' : 'W', depth - 1, true);
                    if (score < bestScore) {
                        bestScore = score;
                    }
                }
            }
        }
        return bestScore;
    }
}
int evaluateBoard(char board[][MAX_BOARD_SIZE], int n) {
    int score = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 'W') {
                score += 1;
            } else if (board[i][j] == 'B') {
                score -= 1;
            }
        }
    }
    return score;
}
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

void updateBoard(char board[][MAX_BOARD_SIZE], int n, int row, int col, char colour) {
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
    char computerColour = (playerColour == 'W') ? 'B' : 'W';

    int bestRow, bestCol;
    int moveStatus = makeMove((const char (*)[MAX_BOARD_SIZE])board, n, computerColour, &bestRow, &bestCol);

    if (moveStatus == 1) {
        updateBoard(board, n, bestRow, bestCol, computerColour);
        printf("Computer places %c at %c%c.\n", computerColour, 'a' + bestRow, 'a' + bestCol);
        printBoard(board, n);
    } else {
        printf("%c player has no valid move.\n", computerColour);
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

    char computerColour = (playerColour == 'W') ? 'B' : 'W';

    board[n / 2 - 1][n / 2 - 1] = 'W';
    board[n / 2][n / 2] = 'W';
    board[n / 2 - 1][n / 2] = 'B';
    board[n / 2][n / 2 - 1] = 'B';

    printBoard(board, n);
    if (playerColour == 'B') {
        computerMove(board, n, computerColour);
    }

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
            updateBoard(board, n, rowIndex, colIndex, computerColour);
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
