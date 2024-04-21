#include <stdio.h>

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#define LEN 256


int main() {
    char *other = malloc(256);
    free(other);

    char string[LEN];
    // while (fgets(string, LEN, stdin) != 0) {
    //     // printf("%s", string);
    // }

    return 0;
}