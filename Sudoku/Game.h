
#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*  The module responsible for logic involving the current state of the
 * sudoku game, as well as the main struct "GameState" used to save the current state of the game. */

/* Enum for the current game mode. Used in GameState. */
typedef enum game_mode {
    EDIT_MODE,
    SOLVE_MODE,
    INIT_MODE
} GAME_MODE;

/* Represents the current state of the sudoku board. */
typedef struct GameState{
    int size;
    int rowsInBlock;
    int colsInBlock;
    int **board;
    bool **fixed;
    bool markErrors;
    GAME_MODE mode;
} GameState;

/* Returns true if the current board is in a legal state. (i.e. not erroneous) */
bool isBoardLegal(GameState *gameState);

/* Returns true if setting value in [row,col] results in a legal state (i.e. not erroneous). */
bool isUserLegalMove(GameState *gameState, int row, int col, int value);

/* Returns true if the board is full */
bool isUserBoardFull(GameState *gameState);

/* Returns number of empty cells in board. */
int countBlanks(GameState *gameState);

/* Returns true if val is in row, col, or block(row,col). */
bool safeMove(int row, int col, int val, GameState *gameState);

/* Copies the content of the values between 2 GameState's boards.
 * This does NOT copy anything else, such as game mode or fixed cells. */
void copyFromBoardToBoard(GameState* gameStateFrom, GameState* gameStateTo);

/* Returns sub-board consisting only of fixed board cells. */
GameState *getFixedGame(GameState *gameState);

/* Getters, setters and creator/destroyer for GameState */
GameState *createGameState(int rowsInBlock, int colsInBlock);
void destroyGameState(GameState *gameState);
void setCellValue (int row, int col, int value, GameState* gameState);
int getCellValue (int row, int col, GameState* gameState);
void setFixed (int row, int col, bool value, GameState* gameState);
bool isFixed (int row, int col, GameState* gameState);
int getSize (GameState* gameState);
int getRowsInBlock (GameState* gameState);
int getColsInBlock (GameState* gameState) ;
void setGameMode(GameState *gameState, GAME_MODE status);
GAME_MODE getGameMode(GameState *gameState);
void setMarkErrors(GameState *gameState, bool val);
bool getMarkErrors(GameState *gameState);


#endif