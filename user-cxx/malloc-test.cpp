#include <cstdlib>
#include <cstdio>

int *fill(int size) {
    int *c = (int *) malloc(80);

    for (int i = 0; i < 10; i++)
        c[i] = i;

    return c;
}

void print(int *c, int size) {
    printf("[");
    for (int i = 0; i < 10; i++)
        printf("%d, ", c[i]);
    printf("]\n");
}

int main(int argc, char **argv) {
    int *c = fill(10);
    print(c, 10);

    free(c);

    exit();

    return 0;
}
