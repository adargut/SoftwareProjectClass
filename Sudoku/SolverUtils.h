
#ifndef SUDOKU_SOLVERUTILS_H
#define SUDOKU_SOLVERUTILS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "MainAux.h"

/* The module provides additional utilities for Solver, mainly stack that replaces recursion. */

/* Stack to emulate recursion for exhaustive backtracking algorithm. */
typedef struct recursion_stack {
    int capacity;
    int top;
    int *rows;
    int *cols;
    bool **visited;
} recursion_stack ;

/* Create stack that replaces recursion while getting #solutions for board. */
recursion_stack* createStack(int capacity);

/* Destroy aforementioned stack. */
void destroyStack(recursion_stack *stack);

/* Check if stack is full (i.e. stack overflow). */
bool isFull(recursion_stack *stack);

/* Check if stack is empty. */
bool isEmpty(recursion_stack *stack);

/* Push new game state to top of the stack. */
bool push(recursion_stack *stack, int row, int col);

/* Removes top element of stack and returns true on success, false otherwise. */
bool pop(recursion_stack *stack);

/* Retrieves top row element of the stack. */
int peekRow(recursion_stack *stack);

/* Retrieves top column element of the stack. */
int peekCol(recursion_stack *stack);

/* Getters and setters for the stack. */
int getNextRow(int size, int row, int col);
int getNextCol(int size, int col);
int getPrevRow(int row, int col);
int getPrevCol(int size, int col);
void setVisited(recursion_stack *stack, bool value, int row, int col);
bool isVisited(recursion_stack *stack, int row, int col);

#endif
