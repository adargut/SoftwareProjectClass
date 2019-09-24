
#ifndef SUDOKU_HISTORYHANDLER_H
#define SUDOKU_HISTORYHANDLER_H
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "MainAux.h"

/* This module handles the history of the board, and enables us to redo/undo a move made by user. */

/* Struct represents changes that occurred in the board by a single move. */
typedef struct HistoryChange {
    int row;
    int col;
    int oldCellValue;
    int newCellValue;
    struct HistoryChange* nextChange;
} HistoryChange;

/* Struct represents historical state of board, including previous moves and changes occurred. */
typedef struct HistoryState {
    HistoryChange* changes;
    struct HistoryState* nextState;
    struct HistoryState* prevState;
} HistoryState;

/* Clears all forward history. */
void clearForwardHistory(HistoryState* historyState);

/* Undoes a move performed by user - updates the gameState. */
void undoMove(HistoryState *historyState, GameState *gameState, bool printEnabled);

/* Redoes a move performed by user - updates the gameState. */
void redoMove(HistoryState* historyState, GameState* gameState, bool printEnabled);

/* Add change to history state, while deleting all forward history. */
void updateHistoryWithChange(HistoryState** historyState, HistoryChange* historyChange);

/* Getters, setters and creators/destroyers for the structs we have declared */
HistoryState* createHistoryState ();
HistoryChange* createHistoryChange(int row, int col, int oldCellValue, int newCellValue);
void destroyHistoryState(HistoryState* historyState);
void destroyAllHistory(HistoryState* historyState);
void destroyAllChanges(HistoryChange* historyChange);
HistoryState* getNextState(HistoryState* historyState);
HistoryState* getPreviousState(HistoryState* historyState);
HistoryChange* getChanges(HistoryState* historyState);
void setNextState(HistoryState* historyState, HistoryState* nextState);
void setPrevState(HistoryState* historyState, HistoryState* prevState);
void setChanges(HistoryState* historyState, HistoryChange* historyChange);

#endif
