
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MainAux.h"
#include "Parser.h"

/* This module provides auxiliary methods for main.c */

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/
int getRandom(int lower, int upper) {
    int res;
    if (lower == upper) {
        return lower;
    }
    /* randomize result */
    res = (rand() % (upper - lower + 1)) + lower;
    return res;
}

void destroyMatrix(int **matrix, int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void checkFullBoard(GameState *gameState) {
    if (isUserBoardFull(gameState)) {
        if (isBoardLegal(gameState)) {
            printf("Congratulations! You successfully completed this puzzle!\n");
            setGameMode(gameState, INIT_MODE);
        } else {
            printf("Unfortunately, there is a mistake in your solution. You can still try to correct it!\n");
        }
    }
}




void printBoard(GameState *gameState) {
    int row;
    int col;
    int counter;
    int value;
    int size = getSize(gameState);
    int rowsInBlock = getRowsInBlock(gameState);
    int colsInBlock = getColsInBlock(gameState);

    for (row = 0; row < size; row++) {
        if (row % rowsInBlock == 0) {
            for (counter = 0; counter < (2 * (rowsInBlock) + 1 + size * 4); counter++) {
                printf("-");
            }
            printf("\n");
        }
        for (col = 0; col < size; col++) {
            if (col % colsInBlock == 0) {
                printf("| ");
            }
            value = getCellValue(row, col, gameState);
            if (value == 0) {
                printf("    ");
            } else {
                if (value < 10) {
                    printf(" ");
                }
                printf("%d", value);
                if (isFixed(row, col, gameState) && getGameMode(gameState) == SOLVE_MODE) {
                    printf(". ");
                } else if ((getMarkErrors(gameState) || getGameMode(gameState) == EDIT_MODE) &&
                           !isUserLegalMove(gameState, row, col, value)) {
                    printf("* ");
                } else {
                    printf("  ");
                }
            }
        }
        printf("|\n");
    }
    for (counter = 0; counter < (2 * (rowsInBlock) + 1 + size * 4); counter++) {
        printf("-");
    }
    printf("\n");
}


int size_t2int(size_t val) {
    return (val <= INT_MAX) ? (int) ((ssize_t) val) : -1;
}

void *safeMalloc(void *ptr) {
    if (ptr == NULL) {
        perror("Couldn't allocate memory! Exiting...\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void START_GAME() {
    /*Initialize*/
    char input[MAX];
    char *parsedInput;
    USER_CHOICE command;
    HistoryState **pHistoryState = safeMalloc(malloc(sizeof(HistoryState *)));
    GameState **pGameState = safeMalloc(malloc(sizeof(GameState *)));
    int i = 0;

    /* Empty gamestate in initmode, represents the beginning of the game */
    *pGameState = createGameState(1, 1);
    *pHistoryState = createHistoryState();

    printf("-----------TAUDOKU-----------\n");
    printf("Enter a command of your choice:\n");

/* Start game. Each loop is a line fed by the user. */
    while (fgets(input, MAX, stdin)) {
        if (strlen(input) > 256) {
            printf("Error: More than 256 characters. Please try again.\n");
            continue;
        }
        i = 0;
        while (input[i] == ' ' || input[i] == '\t' || input[i] == '\r' || input[i] == '\n') {
            if (input[i] == '\n') {
                break;
            }
            i++;
        }
        if (input[i] == '\n') {
            continue;
        }
        parsedInput = strtok(input, "\n");
        command = parseCommand(pGameState, parsedInput);

        executeCommand(pGameState, pHistoryState, command, input);
    }
}
