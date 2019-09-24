#include "ParserUtils.h"

/* The module provides additional utilities for Parser, used for input validation. */

/*----------------------DECLARATIONS----------------------------*/
bool isMode(GameState *gameState, GAME_MODE gameMode);
bool isInt(char *input);
bool isFloat(char *input);
bool paramsCountCorrect(int params, int minParams, int maxParams);
bool paramsAreDigits(char **input, int params);
bool intsInRange(char **input, int params, int paramIdx, int minVal, int maxVal);
bool floatsInRange(char **input, int params, int minVal, int maxVal);

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

USER_CHOICE validateSet(GameState *gameState, int params, char **input) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 3, 3)) {
        printf("Details: <set X Y Z> expects exactly 3 parameters.\n");
        return INVALID_COMMAND;
    }
    if (!paramsAreDigits(input, params)) {
        return INVALID_COMMAND;
    }
    if (!intsInRange(input, 2, 1, 1, getSize(gameState))) {
        printf("<set X Y Z> - X,Y must be between 1 and %d!\n", getSize(gameState));
        return INVALID_COMMAND;
    }
    if (!intsInRange(input, 1, 3, 0, getSize(gameState))) {
        printf("<set X Y Z> - X,Y,Z must be between 0 and %d!\n", getSize(gameState));
        return INVALID_COMMAND;
    }
    return SET;
}

USER_CHOICE validateAutofill(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE) || isMode(gameState, EDIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: <autofill> expects NO parameters.\n");
        return INVALID_COMMAND;
    }
    if (!isBoardLegal(gameState)) {
        throw_illegalCommandForCurrentBoard();
        printf("Details: <autofill> cannot be used on an erroneous board.\n");
        return INVALID_COMMAND;
    }
    return AUTOFILL;
}

USER_CHOICE validateUndo(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: undo expects NO parameters.\n");
        return INVALID_COMMAND;
    }
    return UNDO;
}

USER_CHOICE validateRedo(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: redo expects NO parameters.\n");
        return INVALID_COMMAND;
    }
    return REDO;
}

USER_CHOICE validateReset(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: reset expects NO parameters.\n");
    }
    return RESET;
}

USER_CHOICE validateMarkErrors(GameState *gameState, int params, char **input) {
    if (isMode(gameState, INIT_MODE) || isMode(gameState, EDIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 1, 1) || !intsInRange(input, 1, 1, 0, 1)) {
        printf("Details: mark_errors expects exactly ONE parameter - 1 or 0.\n");
        return INVALID_COMMAND;
    }
    return MARK_ERRORS;
}

USER_CHOICE validateExit(int params) {
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: exit expects NO parameters.\n");
        return INVALID_COMMAND;
    }
    return EXIT;
}

USER_CHOICE validatePrintBoard(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: print_board expects NO parameters.\n");
        return INVALID_COMMAND;
    }
    return PRINT_BOARD;
}

USER_CHOICE validateEdit(int params, char **input) {
    if (!paramsCountCorrect(params, 0, 1)) {
        printf("Details: <edit [X]> may include at most ONE parameter of the file path.\n");
        return INVALID_COMMAND;
    }
    if (params == 1 && !validLoadPath(input[1])) {
        throw_filePathError();
        return INVALID_COMMAND;
    }
    if (params == 1 && !validFileFormat(input[1])) {
        throw_invalidFileFormatError();
        return INVALID_COMMAND;
    }
    return EDIT;
}

USER_CHOICE validateSolve(int params, char **input) {
    if (!paramsCountCorrect(params, 1, 1)) {
        printf("Details: <solve X> must include exactly ONE parameter of the file path.\n");
        return INVALID_COMMAND;
    }
    if (!validLoadPath(input[1])) {
        throw_filePathError();
        return INVALID_COMMAND;
    }
    if (!validFileFormat(input[1])) {
        return INVALID_COMMAND;
    }
    if (!isLegalLoadFixed(input[1])) {
        throw_illegalCommandForCurrentBoard();
        printf("Details: Boards with erroneous fixed cells may not be solved.\n");
        return INVALID_COMMAND;
    }
    return SOLVE;
}

USER_CHOICE validateSave(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 1, 1)) {
        printf("Details: <save X> must include exactly ONE parameter of the file path.\n");
        return INVALID_COMMAND;
    }
    if (getGameMode(gameState) == EDIT_MODE) {
        if (!isBoardLegal(gameState)) {
            throw_illegalCommandForCurrentBoard();
            printf("Details: Board is erroneous in EDIT mode, please fix the board before saving.\n");
            return INVALID_COMMAND;
        }
        if (!isSolvable(gameState)) {
            throw_unsolvableFileError();
            return INVALID_COMMAND;
        }
    }
    return SAVE;
}

USER_CHOICE validateValidate(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: <validate> expects NO parameters.\n");
        return INVALID_COMMAND;
    }
    if (!isBoardLegal(gameState)) {
        throw_illegalCommandForCurrentBoard();
        printf("Details: <validate> cannot be used on an erroneous board. Please fix the board and try again.\n");
        return INVALID_COMMAND;
    }
    return VALIDATE;
}

USER_CHOICE validateGuess(GameState *gameState, int params, char **input) {
    if (isMode(gameState, INIT_MODE) || isMode(gameState, EDIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 1, 1)) {
        printf("Details: <guess X> expects exactly ONE parameter.\n");
        return INVALID_COMMAND;
    }
    if (!floatsInRange(input, params, 0, 1)) {
        return INVALID_COMMAND;
    }

    if (!isBoardLegal(gameState)) {
        throw_illegalCommandForCurrentBoard();
        printf("Details: <guess X> cannot be used on an erroneous board. Please fix the board and try again.\n");
        return INVALID_COMMAND;
    }
    return GUESS;
}

USER_CHOICE validateHint(GameState *gameState, int params, char **input) {
    if (isMode(gameState, INIT_MODE) || isMode(gameState, EDIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 2, 2)) {
        printf("Details: <hint X Y> expects exactly TWO parameters.\n");
        return INVALID_COMMAND;
    }
    if (!paramsAreDigits(input, params)) {
        return INVALID_COMMAND;
    }
    if (!intsInRange(input, params, 1, 1, getSize(gameState))) {
        printf("Details: <hint X Y> - X,Y must be within the board's range!\n");
        return INVALID_COMMAND;
    }
    if (!isBoardLegal(gameState)) {
        printf("Details: <hint X Y> cannot be used on an erroneous board. Please fix the board and try again.\n");
        return INVALID_COMMAND;
    }
    return HINT;
}

USER_CHOICE validateGuessHint(GameState *gameState, int params, char **input) {
    if (isMode(gameState, INIT_MODE) || isMode(gameState, EDIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 2, 2)) {
        printf("Details: <guess_hint X Y> expects exactly TWO parameters.\n");
        return INVALID_COMMAND;
    }
    if (!paramsAreDigits(input, params)) {
        return INVALID_COMMAND;
    }
    if (!intsInRange(input, params, 1, 1, getSize(gameState))) {
        printf("Details: <guess_hint X Y> - X,Y must be within the board's range!\n");
        return INVALID_COMMAND;
    }
    if (!isBoardLegal(gameState)) {
        printf("Details: <guess_hint X Y> cannot be used on an erroneous board. Please fix the board and try again.\n");
        return INVALID_COMMAND;
    }
    return GUESS_HINT;
}

USER_CHOICE validateNumSolutions(GameState *gameState, int params) {
    if (isMode(gameState, INIT_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 0, 0)) {
        printf("Details: <num_solutions> expects exactly ONE parameter.\n");
        return INVALID_COMMAND;
    }
    if (!isBoardLegal(gameState)) {
        throw_illegalCommandForCurrentBoard();
        printf("Details: <num_solutions> may not be used for erroneous boards.\n");
        return INVALID_COMMAND;
    }
    return NUM_SOLUTIONS;
}

USER_CHOICE validateGenerate(GameState *gameState, int params, char **input) {
    char *endPtr;
    if (isMode(gameState, INIT_MODE) || isMode(gameState, SOLVE_MODE)) {
        return INVALID_COMMAND;
    }
    if (!paramsCountCorrect(params, 2, 2)) {
        printf("Details: <generate X Y> expects exactly TWO parameters.\n");
        return INVALID_COMMAND;
    }
    if (!paramsAreDigits(input, params)) {
        return INVALID_COMMAND;
    }
    if (!intsInRange(input, params, 1, 1, getSize(gameState) * getSize(gameState))) {
        return INVALID_COMMAND;
    }
    if (countBlanks(gameState) <= strtol(input[1], &endPtr, 10)) {
        printf("Error: <generate X Y> - X can't be bigger than the amount of blank cells.\n");
    }
    return GENERATE;
}

/*--------------------------------------------------------------*/
/*--------------------PRIVATE FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

/* Check if command matches mode. */
bool isMode(GameState *gameState, GAME_MODE gameMode) {
    if (getGameMode(gameState) == gameMode) {
        throw_illegalCommandForMode(gameMode);
        return true;
    }
    return false;
}

/* Check if parameter count is valid for command chosen. */
bool paramsCountCorrect(int params, int minParams, int maxParams) {
    if (params > maxParams) {
        throw_tooManyParametersError();
        return false;
    }
    if (params < minParams) {
        throw_tooFewParametersError();
        return false;
    }
    return true;
}

/* Check if parameters are digits. */
bool paramsAreDigits(char **input, int params) {
    int i;
    for (i = 1; i <= params; i++) {
        if (!isdigit(*input[i])) {
            throw_illegalParameterValueError();
            printf("Details: Parameter number %d is not a digit.\n", i);
            return false;
        }
    }
    return true;
}

/* Check if params are in legal range for command. */
bool intsInRange(char **input, int params, int paramIdx, int minVal, int maxVal) {
    int i;
    char *endPtr;
    for (i = paramIdx; i < params + paramIdx; i++) {
        if (!isInt(input[i])) {
            printf("Details: Parameter number %d is not an integer.\n", i);
            return false;
        }
        if (strtol(input[i], &endPtr, 10) < minVal || strtol(input[i], &endPtr, 10) > maxVal) {
            throw_illegalParameterRangeError();
            printf("Details: Parameter number %d is not in the correct range of %d to %d.\n", i, minVal, maxVal);
            return false;
        }
    }
    return true;
}

bool floatsInRange(char **input, int params, int minVal, int maxVal) {
    int i;
    for (i = 1; i <= params; i++) {
        if (!isFloat(input[i])) {
            printf("Details: Parameter number %d is not a float or integer.\n", i);
            return false;
        }
        if (atof(input[i]) < minVal || atof(input[i]) > maxVal) {
            throw_illegalParameterRangeError();
            printf("Details: Parameter number %d is not in the correct range of %d to %d.\n", i, minVal, maxVal);
            return false;
        }
    }
    return true;
}

bool isInt(char *input) {
    int i = 0;
    while (input[i] != '\0') {
        if (!(input[i] <= '9' && input[i] >= '0')) {
            return false;
        }
        i++;
    }
    return true;
}

bool isFloat(char *input) {
    int i = 0;
    bool floatingPoint = false;
    while (input[i] != '\0') {
        if (!(input[i] <= '9' && input[i] >= '0')) {
            if (input[i] == '.' && !floatingPoint) {
                floatingPoint = true;
            } else {
                return false;
            }
        }
        i++;
    }
    return true;
}
