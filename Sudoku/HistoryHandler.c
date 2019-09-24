
#include "HistoryHandler.h"

/* This module handles the history of the board, and enables us to redo/undo a move made by user. */

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/


void clearForwardHistory(HistoryState *historyState) {
    HistoryState *tmp;
    if (historyState->nextState == NULL) {
        return;
    }
    else {
        tmp = historyState->nextState;
        historyState->nextState = NULL;
        historyState = tmp;
    }
    while (historyState->nextState != NULL) {
        tmp = historyState->nextState;
        destroyHistoryState(historyState);
        historyState = tmp;
    }
    destroyHistoryState(historyState);
}

void undoMove(HistoryState *historyState, GameState *gameState, bool printEnabled) {
    HistoryChange *changes = getChanges(historyState);
    int row, col, oldVal, newVal;
    while (changes != NULL) {
        row = changes->row;
        col = changes->col;
        newVal = changes->oldCellValue;
        oldVal = changes->newCellValue;
        setCellValue(row, col, newVal, gameState);
        if (printEnabled) {
            printf("Cell [%d,%d] changed from %d to %d.\n", row + 1, col + 1, oldVal, newVal);
        }
        changes = changes->nextChange;
    }
}

void redoMove(HistoryState *historyState, GameState *gameState, bool printEnabled) {
    HistoryChange *changes = getChanges(getNextState(historyState));
    int row, col, oldVal, newVal;
    while (changes != NULL) {
        row = changes->row;
        col = changes->col;
        newVal = changes->newCellValue;
        oldVal = changes->oldCellValue;
        setCellValue(row, col, newVal, gameState);
        if (printEnabled) {
            printf("Cell [%d,%d] changed from %d to %d.\n", row + 1, col + 1, oldVal, newVal);
        }
        changes = changes->nextChange;
    }
}

void updateHistoryWithChange(HistoryState** pHistoryState, HistoryChange* historyChange) {
    HistoryState* tmpHistoryState = createHistoryState();
    clearForwardHistory(*pHistoryState);
    setChanges(tmpHistoryState, historyChange);
    setNextState(*pHistoryState, tmpHistoryState);
    setPrevState(tmpHistoryState, *pHistoryState);
    *pHistoryState = tmpHistoryState;
}


HistoryState *createHistoryState() {
    HistoryState *historyState = safeMalloc(malloc(sizeof(HistoryState)));
    historyState->changes = NULL;
    historyState->nextState = NULL;
    historyState->prevState = NULL;
    return historyState;
}

HistoryChange *createHistoryChange(int row, int col, int oldCellValue, int newCellValue) {
    HistoryChange *historyChange = safeMalloc(malloc(sizeof(HistoryChange)));
    historyChange->row = row;
    historyChange->col = col;
    historyChange->oldCellValue = oldCellValue;
    historyChange->newCellValue = newCellValue;
    historyChange->nextChange = NULL;
    return historyChange;
}

void destroyHistoryState(HistoryState *historyState) {
    if (historyState->changes != NULL) {
        destroyAllChanges(historyState->changes);
    }
    free(historyState);
}

void destroyAllHistory(HistoryState *historyState) {
    HistoryState *tmp = historyState;
    while (historyState->nextState != NULL) {
        historyState = historyState->nextState;
    }
    while (historyState->prevState != NULL) {
        tmp = historyState->prevState;
        destroyHistoryState(historyState);
        historyState = tmp;
    }
    destroyHistoryState(historyState);
}

void destroyAllChanges(HistoryChange *historyChange) {
    HistoryChange *tmp = historyChange;
    while (historyChange->nextChange != NULL) {
        tmp = historyChange->nextChange;
        free(historyChange);
        historyChange = tmp;
    }
    free(historyChange);
}

HistoryState *getNextState(HistoryState *historyState) {
    return historyState->nextState;
}

void setNextState(HistoryState *historyState, HistoryState *nextState) {
    historyState->nextState = nextState;
}

HistoryState *getPreviousState(HistoryState *historyState) {
    return historyState->prevState;
}

void setPrevState(HistoryState *historyState, HistoryState *prevState) {
    historyState->prevState = prevState;
}

HistoryChange *getChanges(HistoryState *historyState) {
    return historyState->changes;
}

void setChanges(HistoryState *historyState, HistoryChange *historyChange) {
    historyState->changes = historyChange;
}
