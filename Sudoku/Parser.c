#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Parser.h"
#include "CommandExecutioner.h"

/*  The module responsible for parsing user commands entered to console. */

/*----------------------DECLARATIONS----------------------------*/
bool matchesFormat(char *str, USER_CHOICE choice);

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

USER_CHOICE parseCommand(GameState **gameState, char *input) {
    int k = 0;
    char *str[MAX];
    char inputCopy[MAX];
    char *token;

    /* Reset contents of array */
    str[1] = NULL;
    strcpy(inputCopy, input);
    token = strtok(inputCopy, " \t\r\n");

    while (token != 0) {
        str[k++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    k--;

    if (matchesFormat(str[0], SET)) {
        return validateSet(*gameState, k, str);
    }
    if (matchesFormat(str[0], AUTOFILL)) {
        return validateAutofill(*gameState, k);
    }
    if (matchesFormat(str[0], HINT)) {
        return validateHint(*gameState, k, str);
    }
    if (matchesFormat(str[0], GUESS)) {
        return validateGuess(*gameState, k, str);
    }
    if (matchesFormat(str[0], GUESS_HINT)) {
        return validateGuessHint(*gameState, k, str);
    }
    if (matchesFormat(str[0], VALIDATE)) {
        return validateValidate(*gameState, k);
    }
    if (matchesFormat(str[0], NUM_SOLUTIONS)) {
        return validateNumSolutions(*gameState, k);
    }
    if (matchesFormat(str[0], GENERATE)) {
        return validateGenerate(*gameState, k, str);
    }
    if (matchesFormat(str[0], UNDO)) {
        return validateUndo(*gameState, k);
    }
    if (matchesFormat(str[0], REDO)) {
        return validateRedo(*gameState, k);
    }
    if (matchesFormat(str[0], RESET)) {
        return validateReset(*gameState, k);
    }
    if (matchesFormat(str[0], MARK_ERRORS)) {
        return validateMarkErrors(*gameState, k, str);
    }
    if (matchesFormat(str[0], EXIT)) {
        return validateExit(k);
    }
    if (matchesFormat(str[0], PRINT_BOARD)) {
        return validatePrintBoard(*gameState, k);
    }
    if (matchesFormat(str[0], EDIT)) {
        return validateEdit(k, str);
    }
    if (matchesFormat(str[0], SOLVE)) {
        return validateSolve(k, str);
    }
    if (matchesFormat(str[0], SAVE)) {
        return validateSave(*gameState, k);
    }
    throw_unknownCommand();
    return INVALID_COMMAND;
}

void executeCommand(GameState **pGameState, HistoryState **pHistoryState, USER_CHOICE commandType, char *input) {
    int k = 0, row, col, val;
    char *str[MAX];
    char inputCopy[MAX];
    char *endPtr;
    char *token;
    /* Reset contents of array */
    str[1] = NULL;
    strcpy(inputCopy, input);
    token = strtok(inputCopy, " \t\r\n");

    while (token != 0) {
        str[k++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    k--;

    switch (commandType) {
        case (EDIT):
            if (k == 1) {
                executeEdit(pGameState, pHistoryState, str[1], /* hasPath = */ true);
            } else {
                executeEdit(pGameState, pHistoryState, "", /* hasPath= */ false);
            }
            return;
        case (SOLVE):
            executeSolve(pGameState, pHistoryState, str[1]);
            return;
        case (SAVE):
            executeSave(*pGameState, str[1]);
            return;
        case (SET):
            col = strtol(str[1], &endPtr, 10) - 1;
            row = strtol(str[2], &endPtr, 10) - 1;
            val = strtol(str[3], &endPtr, 10);
            executeSet(*pGameState, pHistoryState, row, col, val);
            return;
        case (UNDO):
            executeUndo(*pGameState, pHistoryState);
            return;
        case (REDO):
            executeRedo(*pGameState, pHistoryState);
            return;
        case (RESET):
            executeReset(*pGameState, pHistoryState);
            return;
        case (AUTOFILL):
            executeAutofill(*pGameState, pHistoryState);
            return;
        case (VALIDATE):
            executeValidate(*pGameState);
            return;
        case (HINT):
            col = strtol(str[1], &endPtr, 10) - 1;
            row = strtol(str[2], &endPtr, 10) - 1;
            executeHint(*pGameState, row, col);
            return;
        case (GUESS_HINT):
            col = strtol(str[1], &endPtr, 10) - 1;
            row = strtol(str[2], &endPtr, 10) - 1;
            executeGuessHint(*pGameState, row, col);
            return;
        case (GUESS):
            executeGuess(*pGameState, pHistoryState, atof(str[1]));
            return;
        case (GENERATE):
            executeGenerate(*pGameState, pHistoryState, strtol(str[1], &endPtr, 10), strtol(str[2], &endPtr, 10));
            return;
        case (PRINT_BOARD):
            printBoard(*pGameState);
            return;
        case (MARK_ERRORS):
            strcmp(str[1], "0") == 0 ? setMarkErrors(*pGameState, false) : setMarkErrors(*pGameState, true);
            return;
        case (NUM_SOLUTIONS):
            executeNumSolutions(*pGameState);
            return;
        case (EXIT):
            printf("Exiting...\n");
            destroyAllHistory(*pHistoryState);
            free(pHistoryState);
            destroyGameState(*pGameState);
            free(pGameState);
            exit(EXIT_SUCCESS);
        default:
            return;
    }
}

/*--------------------------------------------------------------*/
/*--------------------PRIVATE FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

/* Checks if user input matches given regular expressions */
bool matchesFormat(char *str, USER_CHOICE choice) {

    switch (choice) {
        case SET:
            if (strcmp(str, "set") == 0) {
                return true;
            }
            return false;
        case RESET:
            if (strcmp(str, "reset") == 0) {
                return true;
            }
            return false;
        case EXIT:
            if (strcmp(str, "exit") == 0) {
                return true;
            }
            return false;
        case EDIT:
            if (strcmp(str, "edit") == 0) {
                return true;
            }
            return false;
        case PRINT_BOARD:
            if (strcmp(str, "print_board") == 0) {
                return true;
            }
            return false;
        case SOLVE:
            if (strcmp(str, "solve") == 0) {
                return true;
            }
            return false;
        case UNDO:
            if (strcmp(str, "undo") == 0) {
                return true;
            }
            return false;
        case REDO:
            if (strcmp(str, "redo") == 0) {
                return true;
            }
            return false;
        case AUTOFILL:
            if (strcmp(str, "autofill") == 0) {
                return true;
            }
            return false;
        case MARK_ERRORS:
            if (strcmp(str, "mark_errors") == 0) {
                return true;
            }
            return false;
        case SAVE:
            if (strcmp(str, "save") == 0) {
                return true;
            }
            return false;
        case NUM_SOLUTIONS:
            if (strcmp(str, "num_solutions") == 0) {
                return true;
            }
            return false;

        case GUESS:
            if (strcmp(str, "guess") == 0) {
                return true;
            }
            return false;
        case HINT:
            if (strcmp(str, "hint") == 0) {
                return true;
            }
            return false;
        case GUESS_HINT:
            if (strcmp(str, "guess_hint") == 0) {
                return true;
            }
            return false;
        case VALIDATE:
            if (strcmp(str, "validate") == 0) {
                return true;
            }
            return false;
        case GENERATE:
            if (strcmp(str, "generate") == 0) {
                return true;
            }
            return false;
        default:
            return false;
    }
}
