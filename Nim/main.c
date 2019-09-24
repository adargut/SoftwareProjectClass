#include <stdio.h>
#include <stdlib.h>
#include "sp_nim.h"
#include "main_aux.h"
#include "SPBufferset.h"

#define HEAPS_AMOUNT 32
#define GAME_OVER (chkEmpty(heaps, length) == 0)
#define STATUS heapStatus(heaps, turn, length)

int main(void) {
    /* declarations */
    int valid = 0;
    int turn = 1;
    int length = 0;
    int rem;
    int ind;
    int heaps[HEAPS_AMOUNT];
    int i1;
    int currHeap = 0;

    printf("Enter the number of heaps:\n");

    valid = scanf(" %d", & length); 

    if (valid < 1 || valid == EOF || length > 32 || length < 1) {
        printf("Error: the number of heaps must be between 1 and 32.\n");
        return (1);
    }

    printf("Enter the heap sizes:\n");
    for (i1 = 0; i1 < length; i1++) {
        valid = scanf(" %d", &currHeap); 
        if (valid < 1 || valid == EOF || currHeap <= 0) {
            printf("Error: the size of heap %d should be positive.\n", i1 + 1);
            return EXIT_FAILURE;
        }
        heaps[i1] = currHeap;
    }

    while (!GAME_OVER) {
        STATUS;
        printf("Your turn: please enter the heap index and the number of removed objects:\n");
        PLAYER:

        valid = scanf(" %d %d", &ind, &rem);
	if (valid < 2) 
	{
	    if (valid == EOF) {
		return EXIT_FAILURE; }
            printf("Error: invalid input.\n");
            return EXIT_FAILURE;
        }

        if (ind < 1 || ind > length || rem > heaps[ind-1] || rem <= 0) {
            printf("ERROR: Invalid input.\nPlease enter again the heap index and the number of removed objects:\n");
            goto PLAYER;
        }

        heaps[ind-1] -= rem;
        printf("You take %d objects from heap %d.\n", rem, ind);
        turn++;
        if (GAME_OVER) {
            printf("You win!\n");
            return EXIT_SUCCESS;
        }

        STATUS;
        cpuMove(heaps, length);
        turn++;

        if (GAME_OVER) {
            printf("Computer wins!\n");
            return EXIT_SUCCESS;
        }

    }
    return EXIT_SUCCESS;

}
