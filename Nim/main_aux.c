#include <stdio.h>
#include <stdlib.h>

void heapStatus (int heaps[], int turn, int len) {
    int i;
    printf("In turn %d heap sizes are: ", turn);
    for (i = 0; i < len - 1; i++) {
        printf("h%d=%d ", i + 1, heaps[i]);
    }
    printf("h%d=%d.\n", i + 1, heaps[i]);
}
