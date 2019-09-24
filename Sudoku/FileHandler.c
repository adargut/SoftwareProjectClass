#include "FileHandler.h"
#include "ErrorHandler.h"

/*  The module responsible for loading and saving boards, as well as validations involving such actions. */

/*----------------------DECLARATIONS----------------------------*/
int getNextIdx(char *currLine, int currIdx);
int nextInt(int startIdx, char *string);
bool cellInRange(int cell, int size);
int __cdecl isblank(int _C);

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

bool validLoadPath(char *filePath) {
    FILE *file;
    /* File exists and can be loaded */
    if ((file = fopen(filePath, "r")) == NULL) {
        return false;
    }
    return true;
}

bool isLegalLoadFixed(char *filePath) {
    GameState *gameState = loadFromFile(filePath);
    GameState *fixedGame = getFixedGame(gameState);
    bool isLegal = true;

    if (!isBoardLegal(fixedGame)) {
        isLegal = false;
    }
    destroyGameState(fixedGame);
    destroyGameState(gameState);
    return isLegal;
}

bool validFileFormat(char *filePath) {
    FILE *file = fopen(filePath, "r");
    char *currLine = (char *) safeMalloc(malloc(CHAR_MAX));
    int counter = 0, idx = 0, rowsInlbock = -1, colsInBlock = -1, size = 0, cell = 0, rowsAmount = 0;

    /* Make sure first line has correct format. */
    if (fgets(currLine, sizeof(currLine), file) == NULL) {
        free(currLine);
        throw_invalidFileFormatError();
        printf("Details: First line of file is missing.\n");
        return false;
    }
    for (idx = 0; idx < size_t2int(sizeof(currLine)); idx++) {
        if (currLine[idx] == '\n') {
            break;
        }
        if ((isdigit(currLine[idx]) && isblank(currLine[idx + 1])) ||
            (isdigit(currLine[idx]) && isspace(currLine[idx + 1]))) {
            /* Determine if we have found rowsInBlock element of size or cols in block. */
            if (rowsInlbock < 0) {
                rowsInlbock = currLine[idx] - '0';
            } else {
                colsInBlock = currLine[idx] - '0';
            }
            counter++;
        }
        if (!isdigit(currLine[idx]) && !isspace(currLine[idx]) && !isblank(currLine[idx])) {
            free(currLine);
            throw_invalidFileFormatError();
            printf("Details: Header of file contains illegal character.\n");
            return false;
        }
    }
    /* More than 2 numbers in header files, negative rows in block value or negative cols in block value:
     * these are all sufficient conditions for invalid file format. */
    if (counter != 2 || rowsInlbock < 0 || colsInBlock < 0) {
        free(currLine);
        throw_invalidFileFormatError();
        printf("Details: File contains erroneous first line.\n");
        return false;
    }
    size = rowsInlbock * colsInBlock;
    /* Check all other lines don't contain illegal characters, or cells out of range. */
    while (fgets(currLine, CHAR_MAX, file)) {
        rowsAmount++;
        counter = 0;
        idx = 0;

        while (currLine[idx] != '\n' && currLine[idx] != '\0' ) {
            if (isdigit(currLine[idx])) {
                cell = nextInt(idx, currLine);
                counter++;
                /* Make sure all entries of loaded board aren't too big. */
                if (!cellInRange(cell, size)) {
                    free(currLine);
                    throw_invalidFileFormatError();
                    printf("Details: Body of file contains an element out of range.\n");
                    return false;
                }
                while (isdigit(currLine[idx + 1])) idx++;
            }
            /* Check if we have found an illegal character.
             * Legal characters are integers within range for board, or blanks.*/
            if (!isdigit(currLine[idx]) && !isblank(currLine[idx])
                && currLine[idx] != '.' && !isspace(currLine[idx]) && currLine[idx] != '\0') {
                free(currLine);
                printf("Details: Body of file contains illegal character.\n");
                return false;
            }
            idx++;
        }
        /* A line cannot be too short or too long, e.g. if size is 9 we can't have a line with 4 integers. */
        if (counter != size && counter != 0) {
            free(currLine);
            throw_invalidFileFormatError();
            printf("Details: File contains a row with incorrect amount of columns.\n");
            return false;
        }
        /* Edge case: empty row, can be skipped. */
        if (counter == 0) {
            rowsAmount--;
        }
    }
    if (currLine) {
        free(currLine);
    }
    /* We can't have too few lines in file. */
    if (rowsAmount != size) {
        throw_invalidFileFormatError();
        printf("Details: File contains too many or too little rows.\n");
        return false;
    }
    return true;
}

GameState *loadEmptyBoard() {
    GameState *gameState = createGameState(3, 3);
    return gameState;
}

bool emptyLine(char *line) {
    int pos = 0;
    int counter = 0;
    int len = strlen(line);

    for (pos = 0; pos < len; pos++) {
        if (isdigit(line[pos])) {
            counter++;
        }
    }
    return counter == 0;
}

GameState *loadFromFile(char *filePath) {
    char *rowSize = (char *) safeMalloc(malloc(CHAR_MAX)), *colSize = (char *) safeMalloc(malloc(CHAR_MAX));
    int idx = 0, rows = 0, cols = 0, rowIdx = 0, colIdx = 0, cell = 0, lineIdx = 0;
    char *currLine = (char *) safeMalloc(malloc(CHAR_MAX));
    FILE *loadedGame;
    GameState *newGame;

    /* Load up the game */
    loadedGame = fopen(filePath, "r");
    if (loadedGame == NULL) {
        free(rowSize);
        free(colSize);
        free(currLine);
        return NULL;
    }
    /* Read first line of loaded game into board */
    if (fgets(currLine, CHAR_MAX, loadedGame) == NULL) {
        free(rowSize);
        free(colSize);
        free(currLine);
        return NULL;
    }
    /* Read col/row values from text file. */
    rows = currLine[getNextIdx(currLine, idx++)] - '0';
    cols = currLine[getNextIdx(currLine, idx)] - '0';
    /* Get rid of old board to load up new one */
    newGame = createGameState(rows, cols);
    /* Read board */
    while (fgets(currLine, CHAR_MAX, loadedGame)) {
        lineIdx = 0;

        /* Do not iterate empty lines. */
        if (emptyLine(currLine)) {
            continue;
        }
        /* Iterate line retrieved to update board. */
        for (idx = 0; idx < rows * cols; idx++) {
            lineIdx = getNextIdx(currLine, lineIdx);
            cell = currLine[lineIdx];
            if (isdigit(cell)) {
                cell = nextInt(lineIdx, currLine);
                while (isdigit(currLine[lineIdx])) {
                    lineIdx++;
                }
            } else {
                lineIdx++;
            }
            /* Check if cell needs to be fixed. */
            if ((cell - '0') == -2) {
                setFixed(getPrevRow(rowIdx, colIdx), getPrevCol(rows * cols, colIdx), true, newGame);
                idx--;
            } else {
                setCellValue(rowIdx, colIdx, cell, newGame);
                /* Handle edge case of fixing last column. */
                if (colIdx == rows * cols - 1) {
                    if (currLine[getNextIdx(currLine, lineIdx)] - '0' == -2) {
                        setFixed(rowIdx, colIdx, true, newGame);
                    }
                }
                /* Update row/col indexes. */
                rowIdx = getNextRow(rows * cols, rowIdx, colIdx);
                colIdx = getNextCol(rows * cols, colIdx);
            }
        }
    }
    /* Close the game */
    fclose(loadedGame);
    /* Free resources */
    free(rowSize);
    free(colSize);
    free(currLine);

    return newGame;
}

void saveToFile(char *filePath, GameState *currGame) {
    int rowsInBlock = getRowsInBlock(currGame);
    int colInBlock = getColsInBlock(currGame);
    int rowIdx = 0, colIdx = 0;
    int cell = 0;
    FILE *saveGame = fopen(filePath, "w");

    /* Write rows in block and cols in block to head of saved text file */
    fprintf(saveGame, "%d %d\n", rowsInBlock, colInBlock);

    /* Iterate current game state to load unto file */
    for (rowIdx = 0; rowIdx < getSize(currGame); rowIdx++) {
        for (colIdx = 0; colIdx < getSize(currGame); colIdx++) {
            /* Get current cell */
            cell = getCellValue(rowIdx, colIdx, currGame);

            /* Reached end of the line */
            if (colIdx == getSize(currGame) - 1) {
                fprintf(saveGame, "%d\n", cell);
            } else {
                /* Save cell to file. In EDIT_MODE, all cells are marked as fixed upon saving. */
                if (getGameMode(currGame) == EDIT_MODE) {
                    if (cell != 0) {
                        fprintf(saveGame, "%d. ", cell);
                    } else {
                        fprintf(saveGame, "%d ", cell);
                    }
                }
                /* Save cell to file. In SOLVE_MODE, only fixed cells are marked as fixed upon saving. */
                if (getGameMode(currGame) == SOLVE_MODE) {
                    if (isFixed(rowIdx, colIdx, currGame)) {
                        fprintf(saveGame, "%d. ", cell);
                    } else {
                        fprintf(saveGame, "%d ", cell);
                    }
                }
            }
        }
    }

    /* Close the saved game */
    fclose(saveGame);
}

/*--------------------------------------------------------------*/
/*---------------------PRIVATE FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

/* Utility function for loader: finds next position in line to read cell from. */
int getNextIdx(char *currLine, int currIdx) {
    int i = currIdx + 1;

    if (isdigit(currLine[currIdx]) || currLine[currIdx] == '.') {
        return currIdx;
    }
    /* Find next digit or fixed cell markup or \n in the line. */
    while (currLine[i] == ' ' || currLine[i] == '\t' || currLine[i] == '\r') {
        i++;
    }
    return i;
}

/* Utility function that acts like Java's nextInt from Scanner class. */
int nextInt(int startIdx, char *string) {
    int res = string[startIdx] - '0';

    while (isdigit(string[++startIdx])) {
        res *= 10;
        res += (string[startIdx] - '0');
    }
    return res;
}

/* Check if cell is in range for board. */
bool cellInRange(int cell, int size) {
    if (cell > size) return false;
    return true;
}
