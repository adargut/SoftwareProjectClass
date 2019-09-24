#include "CommandExecutioner.h"
#include "ErrorHandler.h"

/*  This module responsible for executing the game's commands available to the user, after being parsed by the parser. */

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

void executeSet(GameState *gameState, HistoryState **pHistoryState, int row, int col, int val) {
    HistoryChange *historyChange;
    int oldVal = getCellValue(row, col, gameState);
    if (getGameMode(gameState) == EDIT_MODE) {
        setCellValue(row, col, val, gameState);
    } else if (isFixed(row, col, gameState)) {
        printf("This cell is fixed, please try again.\n");
        return;
    } else {
        setCellValue(row, col, val, gameState);
    }
    historyChange = createHistoryChange(row, col, oldVal, val);
    updateHistoryWithChange(pHistoryState, historyChange);
    printBoard(gameState);
    checkFullBoard(gameState);
}

void executeEdit(GameState **gameState, HistoryState **pHistoryState, char *filePath, bool hasPath) {
    destroyGameState(*gameState);
    if (hasPath) {
        *gameState = loadFromFile(filePath);
    } else {
        *gameState = loadEmptyBoard();
    }
    if (*gameState == NULL) {
        perror("Fatal error occurred while opening file! Exiting...\n");
        destroyHistoryState(*pHistoryState);
        free(pHistoryState);
        destroyGameState(*gameState);
        free(gameState);
        exit(EXIT_FAILURE);
    }
    setGameMode(*gameState, EDIT_MODE);
    destroyAllHistory(*pHistoryState);
    *pHistoryState = createHistoryState();
    printBoard(*gameState);
}

void executeSolve(GameState **gameState, HistoryState **pHistoryState, char *filePath) {
    destroyGameState(*gameState);
    *gameState = loadFromFile(filePath);
    if (*gameState == NULL) {
        perror("Fatal error occurred while opening file! Exiting...\n");
        destroyHistoryState(*pHistoryState);
        free(pHistoryState);
        destroyGameState(*gameState);
        free(gameState);
        exit(EXIT_FAILURE);
    }
    setGameMode(*gameState, SOLVE_MODE);
    destroyAllHistory(*pHistoryState);
    *pHistoryState = createHistoryState();
    printBoard(*gameState);
}

void executeSave(GameState *gameState, char *filePath) {
    saveToFile(filePath, gameState);
    printf("Filed saved successfully.\n");
}

void executeUndo(GameState *gameState, HistoryState **pHistoryState) {
    if (getPreviousState(*pHistoryState) == NULL) {
        throw_nothingToUndo();
        return;
    }
    undoMove(*pHistoryState, gameState, /* printEnabled= */ true);
    *pHistoryState = getPreviousState(*pHistoryState);
    printBoard(gameState);
}

void executeRedo(GameState *gameState, HistoryState **pHistoryState) {
    if (getNextState(*pHistoryState) == NULL) {
        throw_nothingToRedo();
        return;
    }
    redoMove(*pHistoryState, gameState, /* printEnabled= */ true);
    *pHistoryState = getNextState(*pHistoryState);
    printBoard(gameState);
}

void executeReset(GameState *gameState, HistoryState **pHistoryState) {
    bool shouldPrintBoard = false;
    while (getPreviousState(*pHistoryState) != NULL) {
        undoMove(*pHistoryState, gameState, /* printEnabled= */ false);
        *pHistoryState = getPreviousState(*pHistoryState);
        shouldPrintBoard = true;
    }
    if (shouldPrintBoard) {
        printBoard(gameState);
    }
}

void executeAutofill(GameState *gameState, HistoryState **pHistoryState) {
    int row, col, val, singleVal, counter;
    HistoryChange *tmpHistoryChange;
    HistoryChange *historyChange = NULL;
    for (row = 0; row < getSize(gameState); row++) {
        for (col = 0; col < getSize(gameState); col++) {
            if (getCellValue(row, col, gameState) == 0) {
                counter = 0;
                for (val = 1; val <= getSize(gameState); val++) {
                    if (safeMove(row, col, val, gameState)) {
                        counter++;
                        singleVal = val;
                        if (counter == 2) {
                            continue;
                        }
                    }
                }
                /* Check if there's only 1 legal value for this empty cell. */
                if (counter == 1) {
                    if (historyChange == NULL) {
                        historyChange = createHistoryChange(row, col, 0, singleVal);
                        tmpHistoryChange = historyChange;
                    } else {
                        tmpHistoryChange->nextChange = createHistoryChange(row, col, 0, singleVal);
                        tmpHistoryChange = tmpHistoryChange->nextChange;
                    }
                }
            }
        }
    }
    if (historyChange != NULL) {
        updateHistoryWithChange(pHistoryState, historyChange);
        /* This is the part that actually writes the filled values to the board */
        redoMove(getPreviousState(*pHistoryState), gameState, /* printEnabled= */ true);
        printBoard(gameState);
        checkFullBoard(gameState);
    } else {
        printf("No obvious cells - no cells were filled.\n");
    }
}

void executeNumSolutions(GameState *gameState) {
    printf("The number of possible solutions to this board is: %d.\n", solutionCounter(gameState));
}

void executeValidate(GameState *gameState) {
    if (isSolvable(gameState)) {
        printf("The board is solvable.\n");
    } else {
        throw_boardUnsolvable();
    }
}

void executeHint(GameState *gameState, int row, int col) {
    SolutionContainer *solutionContainer = getSolution(gameState, ILP);
    if (!solutionContainer->solutionFound) {
        throw_boardUnsolvable();
        destroySolutionContainer(solutionContainer);
        return;
    }
    printf("Hint: Fill cell [%d,%d] with value %d.\n", row + 1, col + 1,
           getValueFromILPSolution(solutionContainer, row, col));
    destroySolutionContainer(solutionContainer);
}

void executeGuessHint(GameState *gameState, int row, int col) {
    int value, idx;
    SolutionContainer *solutionContainer = getSolution(gameState, LP);
    if (!solutionContainer->solutionFound) {
        throw_boardUnsolvable();
        destroySolutionContainer(solutionContainer);
        return;
    }
    printf("Possible values for [%d,%d] are:\n", row + 1, col + 1);
    for (value = 1; value <= solutionContainer->boardSize; value++) {
        idx = getIndexOfVariable(solutionContainer, row, col, value);
        if (idx != -1 && solutionContainer->solution[idx] > 0) {
            printf("%d with a score of %.2f.\n", value, solutionContainer->solution[idx]);
        }
    }
    destroySolutionContainer(solutionContainer);
}


void executeGuess(GameState *gameState, HistoryState **pHistoryState, float threshold) {
    int row, col, value, idx, legalValsCount;
    int randomVal = 0;
    double randomDouble;
    double *legalVals;
    double sum;
    HistoryChange *tmpHistoryChange = NULL;
    HistoryChange *historyChange = NULL;
    SolutionContainer *solutionContainer = getSolution(gameState, LP);
    if (!solutionContainer->solutionFound) {
        throw_boardUnsolvable();
        destroySolutionContainer(solutionContainer);
        return;
    }
    for (row = 0; row < getSize(gameState); row++) {
        for (col = 0; col < getSize(gameState); col++) {
            if (getCellValue(row, col, gameState) == 0) {
                legalValsCount = 0;
                legalVals = calloc(getSize(gameState), sizeof(double));
                for (value = 1; value <= getSize(gameState); value++) {
                    idx = getIndexOfVariable(solutionContainer, row, col, value);
                    if (idx != -1 && solutionContainer->solution[idx] > threshold &&
                        isUserLegalMove(gameState, row, col, value)) {
                        legalVals[value - 1] = solutionContainer->solution[idx];
                        legalValsCount++;
                    }
                }
                if (legalValsCount == 0) {
                    free(legalVals);
                    continue;
                }
                /* Normalize the values in intervals such that each interval is in the size of the probability. */
                sum = 0.0;
                for (value = 1; value <= getSize(gameState); value++) {
                    sum += legalVals[value - 1];
                    legalVals[value - 1] = sum;
                }
                /* Randomize a double between 0 and sum. */
                randomDouble = (rand() / (double) RAND_MAX) * sum;
                /* Pick the selected number. */
                for (value = 1; value <= getSize(gameState); value++) {
                    if (randomDouble <= legalVals[value - 1]) {
                        randomVal = value;
                        break;
                    }
                }
                /* Set the selected number. */
                setCellValue(row, col, randomVal, gameState);
                if (historyChange == NULL) {
                    historyChange = createHistoryChange(row, col, 0, randomVal);
                    tmpHistoryChange = historyChange;
                } else {
                    tmpHistoryChange->nextChange = createHistoryChange(row, col, 0, randomVal);
                    tmpHistoryChange = tmpHistoryChange->nextChange;
                }

                free(legalVals);
            }
        }
    }
    if (historyChange != NULL) {
        updateHistoryWithChange(pHistoryState, historyChange);
    }
    printBoard(gameState);
    checkFullBoard(gameState);
}

void executeGenerate(GameState *gameState, HistoryState **pHistoryState, int toFill, int cellsLeft) {
    HistoryChange *tmpHistoryChange = NULL;
    HistoryChange *historyChange = NULL;
    SolutionContainer *solutionContainer = NULL;
    int attempt = 0, cleared = 0;
    int row, col, value, filled, legalValuesCount, randomVal, idx, oldCellVal, newCellVal;
    int toClear = getSize(gameState) * getSize(gameState) - cellsLeft;
    int *legalValues;
    bool attemptSuccessful;
    GameState *cpyGameState = NULL;

    while (attempt < 1000) {
        /* Initialize */
        filled = 0;
        attemptSuccessful = true;
        cpyGameState = createGameState(getRowsInBlock(gameState), getColsInBlock(gameState));
        copyFromBoardToBoard(gameState, cpyGameState);
        /* Attempt to fill cells randomly */
        while (filled < toFill) {
            row = getRandom(0, getSize(cpyGameState) - 1);
            col = getRandom(0, getSize(cpyGameState) - 1);
            if (getCellValue(row, col, cpyGameState) != 0) {
                continue;
            }
            legalValues = safeMalloc(malloc(getSize(cpyGameState) * sizeof(int)));
            legalValuesCount = 0;
            for (value = 1; value <= getSize(cpyGameState); value++) {
                if (isUserLegalMove(cpyGameState, row, col, value)) {
                    legalValues[legalValuesCount++] = value;
                }
            }
            if (legalValuesCount == 0) {
                attemptSuccessful = false;
                free(legalValues);
                break;
            }
            randomVal = legalValues[getRandom(0, legalValuesCount - 1)];
            setCellValue(row, col, randomVal, cpyGameState);
            filled++;
            free(legalValues);
        }
        if (!attemptSuccessful) {
            attempt++;
            destroyGameState(cpyGameState);
            continue;
        }
        /* Attempt to solve the remaining board using ILP */
        solutionContainer = getSolution(cpyGameState, ILP);
        if (!solutionContainer->solutionFound) {
            attempt++;
            destroyGameState(cpyGameState);
            destroySolutionContainer(solutionContainer);
            continue;
        }
        /* Generate successful */
        break;
    }
    if (attempt == 1000) {
        printf("Error: Generate failed to generate a board after 1,000 attempts.\n");
        return;
    }
    /* Fill cells with solution from ILP */
    for (row = 0; row < getSize(cpyGameState); row++) {
        for (col = 0; col < getSize(cpyGameState); col++) {
            for (value = 1; value <= getSize(cpyGameState); value++) {
                idx = getIndexOfVariable(solutionContainer, row, col, value);
                if (idx != -1 && solutionContainer->solution[idx] == 1.0) {
                    setCellValue(row, col, value, cpyGameState);
                }
            }
        }
    }
    /* Clear toClear cells. */
    while (cleared < toClear) {
        row = getRandom(0, getSize(gameState) - 1);
        col = getRandom(0, getSize(gameState) - 1);
        if (getCellValue(row, col, cpyGameState) == 0) {
            continue;
        } else {
            setCellValue(row, col, 0, cpyGameState);
            cleared++;
        }
    }
    /* Update History and Gamestate */
    for (row = 0; row < getSize(gameState); row++) {
        for (col = 0; col < getSize(gameState); col++) {
            oldCellVal = getCellValue(row, col, gameState);
            newCellVal = getCellValue(row, col, cpyGameState);
            if (oldCellVal != newCellVal) {
                setCellValue(row, col, newCellVal, gameState);
                if (historyChange == NULL) {
                    historyChange = createHistoryChange(row, col, oldCellVal, newCellVal);
                    tmpHistoryChange = historyChange;
                } else {
                    tmpHistoryChange->nextChange = createHistoryChange(row, col, oldCellVal, newCellVal);
                    tmpHistoryChange = tmpHistoryChange->nextChange;
                }
            }
        }
    }
    if (historyChange != NULL) {
        updateHistoryWithChange(pHistoryState, historyChange);
    }
    printBoard(gameState);
    checkFullBoard(gameState);
}
