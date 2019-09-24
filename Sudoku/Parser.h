
#ifndef SUDOKU_PARSER_H
#define SUDOKU_PARSER_H

#include <stdbool.h>
#include "Game.h"
#include "ParserUtils.h"
#define MAX 1024

/*  The module responsible for parsing user commands entered to console, and calling the correct execution method from CommandExecutioner. */

/* Scans user's input and returns it as String format */
USER_CHOICE parseCommand(GameState **gameState, char *input);

/* Execute command chosen by user */
void executeCommand(GameState **pGameState, HistoryState** pHistoryState, USER_CHOICE commandType, char *input);

#endif

