#include "Game.h"
#include "MainAux.h"

/*  The module responsible for logic involving the current state of the
 * sudoku game, as well as the main struct "GameState" used to save the current state of the game. */

/*----------------------DECLARATIONS----------------------------*/
bool safeMoveRow(int row, int val, GameState *gameState);
bool safeMoveCol(int col, int val, GameState *gameState);
bool safeMoveBlock(int block, int val, GameState *gameState);
int findBlock(int row, int col, GameState *gameState);

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

bool isBoardLegal(GameState *gameState) {
    int row, col;
    for (row = 0; row < getSize(gameState); row++) {
        for (col = 0; col < getSize(gameState); col++) {
            if (getCellValue(row, col, gameState) != 0 &&
                !isUserLegalMove(gameState, row, col, getCellValue(row, col, gameState))) {
                return false;
            }
        }
    }
    return true;
}

bool isUserLegalMove(GameState *gameState, int row, int col, int value) {
    int oldValue = getCellValue(row, col, gameState);
    setCellValue(row, col, 0, gameState);
    if (value == 0 || safeMove(row, col, value, gameState)) {
        setCellValue(row, col, oldValue, gameState);
        return true;
    }
    setCellValue(row, col, oldValue, gameState);
    return false;
}

bool isUserBoardFull(GameState *gameState) {
    if (countBlanks(gameState) == 0) {
        return true;
    }
    return false;
}

int countBlanks(GameState *gameState) {
    int row, col, count = 0;
    for (row = 0; row < getSize(gameState); row++) {
        for (col = 0; col < getSize(gameState); col++) {
            if (getCellValue(row, col, gameState) == 0) {
                count++;
            }
        }
    }
    return count;
}

void copyFromBoardToBoard(GameState *gameStateFrom, GameState *gameStateTo) {
    int row, col;
    if (getSize(gameStateFrom) != getSize(gameStateTo)) {
        return;
    }
    for (row = 0; row < getSize(gameStateFrom); row++) {
        for (col = 0; col < getSize(gameStateFrom); col++) {
            setCellValue(row, col, getCellValue(row, col, gameStateFrom), gameStateTo);
        }
    }
}

GameState *createGameState(int rowsInBlock, int colsInBlock) {
    int i;
    GameState *gameState = safeMalloc(malloc(sizeof(GameState)));
    int size = rowsInBlock * colsInBlock;
    gameState->size = size;
    gameState->rowsInBlock = rowsInBlock;
    gameState->colsInBlock = colsInBlock;
    gameState->markErrors = true;
    gameState->mode = INIT_MODE;
    gameState->board = (int **) safeMalloc(malloc(size * sizeof(int *)));
    for (i = 0; i < size; i++) {
        gameState->board[i] = (int *) calloc(size, sizeof(int));
    }
    gameState->fixed = (bool **) safeMalloc(malloc(size * sizeof(bool *)));
    for (i = 0; i < size; i++) {
        gameState->fixed[i] = (bool *) calloc(size, sizeof(bool));
    }
    return gameState;
}

void destroyGameState(GameState *gameState) {
    destroyMatrix(gameState->board, gameState->size);
    destroyMatrix((int **) gameState->fixed, gameState->size);
    free(gameState);
}

GameState *getFixedGame(GameState *gameState) {
    int size = getSize(gameState), rows = getRowsInBlock(gameState), cols = getColsInBlock(gameState);
    GameState *fixedGame = createGameState(rows, cols);
    int rowIdx = 0, colIdx = 0, cell = 0;

    /* Fill in new game state clone with only fixed cells. */
    for (rowIdx = 0; rowIdx < size; rowIdx++) {
        for (colIdx = 0; colIdx < size; colIdx++) {
            if (isFixed(rowIdx, colIdx, gameState)) {
                cell = getCellValue(rowIdx, colIdx, gameState);
            }
            else {
                cell = 0;
            }
            setCellValue(rowIdx, colIdx, cell, fixedGame);
        }
    }
    return fixedGame;
}

void setCellValue(int row, int col, int value, GameState *gameState) {
    gameState->board[row][col] = value;
}

int getCellValue(int row, int col, GameState *gameState) {
    return gameState->board[row][col];
}

void setFixed(int row, int col, bool value, GameState *gameState) {
    gameState->fixed[row][col] = value;
}

bool isFixed(int row, int col, GameState *gameState) {
    return gameState->fixed[row][col];
}

int getSize(GameState *gameState) {
    return gameState->size;
}

int getRowsInBlock(GameState *gameState) {
    return gameState->rowsInBlock;
}

void setMarkErrors(GameState *gameState, bool val) {
    gameState->markErrors = val;
}

bool getMarkErrors(GameState *gameState) {
    return gameState->markErrors;
}

int getColsInBlock(GameState *gameState) {
    return gameState->colsInBlock;
}

void setGameMode(GameState *gameState, GAME_MODE status) {
    gameState->mode = status;
}

GAME_MODE getGameMode(GameState *gameState) {
    return gameState->mode;
}

bool safeMove(int row, int col, int val, GameState *gameState) {
    int block = findBlock(row, col, gameState);

    return safeMoveRow(row, val, gameState) &&
           safeMoveCol(col, val, gameState) &&
           safeMoveBlock(block, val, gameState) &&
           val > 0 && val <= getSize(gameState);
}

/*--------------------------------------------------------------*/
/*--------------------PRIVATE FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

/* Returns true if val is in row. */
bool safeMoveRow(int row, int val, GameState *gameState) {
    int col;
    for (col = 0; col < getSize(gameState); col++) {
        if (getCellValue(row, col, gameState) == val)
            return false; /* val exists in row */
    }
    return true;
}

/* Returns true if val is in col. */
bool safeMoveCol(int col, int val, GameState *gameState) {
    int row;

    for (row = 0; row < getSize(gameState); row++) {
        if (getCellValue(row, col, gameState) == val)
            return false; /* val exists in column */
    }
    return true;
}

/* Returns true if val is in block. */
bool safeMoveBlock(int block, int val, GameState *gameState) {
    int row, col;
    int fromRow = (block / getRowsInBlock(gameState)) * getRowsInBlock(gameState);
    int fromCol = (block % getRowsInBlock(gameState)) * getColsInBlock(gameState);
    for (row = fromRow; row < fromRow + getRowsInBlock(gameState); row++) {
        for (col = fromCol; col < fromCol + getColsInBlock(gameState); col++) {
            if (getCellValue(row, col, gameState) == val) { /* val exists in block */
                return false;
            }
        }
    }
    return true;
}

/* Returns the index of block(row,col), when 0 is the upper left block. */
int findBlock(int row, int col, GameState *gameState) {
    int rows = getRowsInBlock(gameState);
    int cols = getColsInBlock(gameState);
    return (row / rows) * rows + (col / cols);
}


