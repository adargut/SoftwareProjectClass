
#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include <stdbool.h>
#include "gurobi_c.h"
#include "Game.h"
#include "SolverUtils.h"

/*
 * This module verifies the board correction and manages the error status of each cell.
 *
 * We utilise Gurobi's implementation of Simplex algorithm and exhaustive backtracking with stack for recursion.
 */

/* This struct represents a container for the LP. */
typedef struct SolutionContainer {
    double *solution;
    int ***variables;
    int numOfVariables;
    int error;
    int boardSize;
    bool solutionFound;

} SolutionContainer;

/* This enum represents the linear method used for solving the board. */
typedef enum LinearMethod {
    ILP,
    LP
} LinearMethod;

/* Use exhaustive backtracking algorithm (using stack) to count the number of solutions for the board. */
int solutionCounter(GameState *gameState);

/* This function determines whether or not current board is solvable. */
bool isSolvable(GameState *gameState);

/* Getters, setters and creator/destroyer for SolutionContainer & Gurobi */
SolutionContainer *getSolution(GameState *gameState, LinearMethod linearMethod);
SolutionContainer *createSolutionContainer(int size);
void destroySolutionContainer(SolutionContainer* solutionContainer);
void destroyGurobi(GRBenv* env, GRBmodel* model, double* obj, char* variableTypes, int* ind, double* val);
int getValueFromILPSolution(SolutionContainer* solutionContainer, int row, int col) ;
int getIndexOfVariable(SolutionContainer* solutionContainer, int row, int col, int val);
void setIndexOfVariable(SolutionContainer* solutionContainer, int row, int col, int val, int idx);


#endif

