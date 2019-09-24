
#ifndef SUDOKU_PARSERUTILS_H
#define SUDOKU_PARSERUTILS_H

#include "CommandExecutioner.h"
#include "ErrorHandler.h"

/* This module is used by Parser.c to validate each specific command before executing it. */

USER_CHOICE validateSet (GameState *gameState, int params, char** input);

USER_CHOICE validateAutofill(GameState* gameState, int params);

USER_CHOICE validateUndo(GameState* gameState, int params);

USER_CHOICE validateRedo(GameState* gameState, int params);

USER_CHOICE validateReset(GameState* gameState, int params);

USER_CHOICE validateMarkErrors(GameState* gameState, int params, char **input);

USER_CHOICE validateExit(int params);

USER_CHOICE validatePrintBoard(GameState* gameState, int params);

USER_CHOICE validateEdit(int params, char** input);

USER_CHOICE validateSolve(int params, char** input);

USER_CHOICE validateSave(GameState* gameState, int params);

USER_CHOICE validateValidate (GameState* gameState, int params);

USER_CHOICE validateGuess(GameState *gameState, int params, char** input);

USER_CHOICE validateHint(GameState *gameState, int params, char **input);

USER_CHOICE validateGuessHint(GameState *gameState, int params, char **input);

USER_CHOICE validateNumSolutions(GameState *gameState, int params);

USER_CHOICE validateGenerate(GameState *gameState, int params, char** input);

#endif
