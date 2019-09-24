#include "SolverUtils.h"

/* The module provides additional utilities for Solver, mainly stack that replaces recursion. */

/*--------------------------------------------------------------*/
/*---------------------PUBLIC FUNCTIONS-------------------------*/
/*--------------------------------------------------------------*/

/* Initialize stack of given capacity. */
struct recursion_stack *createStack(int capacity) {
    struct recursion_stack *stack = safeMalloc(malloc(sizeof(recursion_stack)));
    int idx = 0, i = 0;

    /* Initially, top index is 0. Index of -1 denotes empty stack. */
    stack->top = 0;
    stack->capacity = capacity;
    /* Initialize rows/columns arrays. */
    stack->rows = (int *) safeMalloc(malloc(capacity * sizeof(int)));
    stack->cols = (int *) safeMalloc(malloc(capacity * sizeof(int)));

    /* Set entries to default values 0. */
    for (idx = 0; idx < capacity; idx++) {
        stack->rows[idx] = 0;
        stack->cols[idx] = 0;
    }

    /* Initialize visited matrix */
    stack->visited = (bool **) safeMalloc(malloc(capacity * sizeof(bool *)));
    for (i = 0; i < capacity; i++) {
        stack->visited[i] = (bool *) calloc(capacity, sizeof(bool));
    }

    return stack;
}

void destroyStack(recursion_stack *stack) {
    destroyMatrix((int **) stack->visited, stack->capacity);
    free(stack->rows);
    free(stack->cols);
    free(stack);
}

bool isFull(recursion_stack *stack) {
    if (stack->top == stack->capacity) {
        return true;
    }
    return false;
}

bool isEmpty(recursion_stack *stack) {
    if (stack->top == -1) {
        return true;
    }
    return false;
}

bool push(recursion_stack *stack, int row, int col) {
    /* Avoid stack overflow. */
    if (isFull(stack)) {
        return false;
    }
    ++stack->top;
    stack->rows[stack->top] = row;
    stack->cols[stack->top] = col;
    return true;
}

bool pop(recursion_stack *stack) {
    /* Avoid stack underflow. */
    if (isEmpty(stack)) {
        return false;
    }
    --stack->top;
    return true;
}

void setVisited(recursion_stack *stack, bool value, int row, int col) {
    stack->visited[row][col] = value;
}

bool isVisited(recursion_stack *stack, int row, int col) {
    return stack->visited[row][col];
}

int peekRow(recursion_stack *stack) {
    return stack->rows[stack->top];
}

int peekCol(recursion_stack *stack) {
    return stack->cols[stack->top];
}

int getNextRow(int size, int row, int col) {
    if (col < size - 1) {
        return row;
    } else {
        return row + 1;
    }
}

int getNextCol(int size, int col) {
    if (col < size - 1) {
        return col + 1;
    } else {
        return 0;
    }
}

int getPrevRow(int row, int col) {
    if (col == 0) {
        return row - 1;
    }
    return row;
}

int getPrevCol(int size, int col) {
    if (col == 0) {
        return size - 1;
    }
    return col - 1;
}
