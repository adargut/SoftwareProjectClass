#include <stdlib.h>
#include "MainAux.h"

int main(int argc, char *argv[]) {
    /* Get seed for random. */
    if (argc == 2) {
        srand((unsigned) atoi(argv[1]));
    } else {
        srand(5);
    }

    /* start playing... */
    START_GAME();

    return 0;
}
