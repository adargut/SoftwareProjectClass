
#ifndef SUDOKU_FILEHANDLER_H
#define SUDOKU_FILEHANDLER_H

#include <limits.h> /* PATH_MAX comes from here */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include "Game.h"
#include "Solver.h"
#include "MainAux.h"
#define MAX 1024

/*  The module responsible for loading and saving boards, as well as validations involving such actions. */

/* Function to save a board to a file at a given path. Paths can be relative or absolute. */
void saveToFile (char *filePath, GameState *gameState);

/* Function to load a board from a given path */
GameState *loadFromFile (char *filePath);

/* Function to load an empty default board */
GameState *loadEmptyBoard ();

/* Check if line received is empty, in order to skip it. */
bool emptyLine(char *line);

/* When loading a board for solve, check no conflicting fixed cells exist */
bool isLegalLoadFixed(char *filePath);

/* Check if given path is valid for loading a file. */
bool validLoadPath (char *filePath);

/* Check if loaded file has correct format. */
bool validFileFormat (char *filePath);


#endif
