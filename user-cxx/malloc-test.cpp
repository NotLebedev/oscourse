#include <stdlib.h>

int
main(int argc, char **argv) {
    int *c = (int *) malloc(80);

    for (int i = 0; i < 10; i++)
        c[i] = i;

    free(c);

    exit();

    return 0;
}
