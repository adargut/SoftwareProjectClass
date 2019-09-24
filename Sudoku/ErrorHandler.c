#include "ErrorHandler.h"

/*  This module provides a collection of fixed error messages, aimed to address various possible illegal states.
 *  Some of the more detailed errors that are more case specific are NOT included in this module, but are rather a part of the parser. */

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

void throw_filePathError() {
    printf("ERROR: Incorrect file path given! Please try again.\n");
}

void throw_unsolvableFileError() {
    printf("ERROR: Unsolvable boards may not be saved! Please try again.\n");
}

void throw_invalidFileFormatError() {
    printf("ERROR: Incorrect file format given! Please try again.\n");
}

void throw_illegalParameterRangeError() {
    printf("ERROR: Parameters given are not in range for this command! Please try again.\n");
}

void throw_illegalParameterValueError() {
    printf("ERROR: Parameters given have illegal values for current board state. Please try again.\n");
}

void throw_tooManyParametersError() {
    printf("ERROR: Too many parameters for given command. Please try again.\n");
}

void throw_tooFewParametersError() {
    printf("ERROR: Too few parameters for given command. Please try again.\n");
}

/* TODO: maybe add which commands are available in each mode */
void throw_illegalCommandForMode(GAME_MODE gameMode) {
    switch (gameMode) {
        case (INIT_MODE):
            printf("ERROR: Command is illegal for INIT board mode. Only available commands in this mode are solve and edit. Please try again.\n");
            break;
        case (EDIT_MODE):
            printf("ERROR: Command is illegal for EDIT board mode. Please try again.\n");
            break;
        case (SOLVE_MODE):
            printf("ERROR: Command is illegal for SOLVE board mode. Please try again.\n");
            break;
        default:
            break;
    }
}

void throw_illegalCommandForCurrentBoard() {
    printf("ERROR: This command is illegal for the current state of the board.\n");
}

void throw_unknownCommand() {
    printf("ERROR: This command doesn't exist. The available commands are:\n solve, edit, mark_errors,\n print_board, set,"
           " validate,\n guess, generate, undo,\n redo, save, hint,\n guess_hint, num_solutions, autofill,\n reset, exit.\n");
}

void throw_nothingToUndo() {
    printf("ERROR: There is no move to undo.\n");
}

void throw_nothingToRedo() {
    printf("ERROR: There is no move to redo.\n");
}

void throw_boardUnsolvable() {
    printf("ERROR: The board is unsolvable in its current state.\n");
}
