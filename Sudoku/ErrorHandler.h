
#ifndef SUDOKU_ERRORHANDLER_H
#define SUDOKU_ERRORHANDLER_H

#include <stdio.h>
#include "Game.h"

/*  This module provides a collection of fixed error messages, aimed to address various possible illegal states.
 *  Some of the more detailed errors that are more case specific are NOT included in this module, but are rather a part of the parser. */

/* Below are errors regarding loading/saving a board from/to a text file */
void throw_invalidFileFormatError();
void throw_loadedInWrongModeError();
void throw_filePathError();
void throw_unsolvableFileError();

/* Below are errors regarding the command that was given as input */
void throw_illegalCommandNameError();
void throw_illegalParameterRangeError();
void throw_illegalParameterValueError();
void throw_tooManyParametersError();
void throw_tooFewParametersError();
void throw_illegalCommandForMode(GAME_MODE gameMode);
void throw_illegalCommandForCurrentBoard();
void throw_unknownCommand();
void throw_nothingToUndo();
void throw_nothingToRedo();
void throw_boardUnsolvable();

#endif
