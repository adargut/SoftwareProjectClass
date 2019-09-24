
#include "sp_nim.h"
#include <stdio.h>
#include <stdlib.h>

int chkEmpty (int heaps[], int len) {
    int i = 0;
    for (i = 0; i < len; i ++) {
        if (heaps[i] != 0) {
            return 1; 
        }
    }
    return 0;
}

void cpuMove (int heaps[], int length) {
    int tmp;
    int nimSum = heaps[0];
    int objects = 0;
    int winnerHeap = -1;
    int i;
    for (i = 1; i < length; i++) {
        nimSum ^= heaps[i]; 
    }
    if (nimSum != 0) {
        for (i = 0; i < length; i++) {
            tmp = heaps[i] ^ nimSum;
            if (tmp < heaps[i]) {
                objects = heaps[i] - (heaps[i] ^ nimSum);
                winnerHeap = i; 
                heaps[i] -= objects; 
                break;
            }
        }
    }
    else {
        for (i = 0; i < length; i++) {
            if (heaps[i] != 0) {
                objects = 1;
                winnerHeap = i; 
                heaps[i] -= objects; 
                break;
            }
        }
    }

    printf("Computer takes %d objects from heap %d.\n", objects, winnerHeap + 1);

}
