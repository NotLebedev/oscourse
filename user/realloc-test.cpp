#include <cstdlib>
#include <cstdio>

int *fill(int size) {
    int *c = (int *) malloc(sizeof (*c));
    c[0] = 13;
    c = (int *) realloc(c, size * sizeof(*c));
    if (c == nullptr)
        return nullptr;

    for (int i = 1; i < size; i++)
        c[i] = i;

    return c;
}

void print(int *c, int size) {
    printf("[");
    for (int i = 0; i < size; i++)
        printf("%d, ", c[i]);
    printf("]\n");
}

int main(int argc, char **argv) {
    int *c = fill(10);
    print(c, 10);

    free(c);

    c = fill(100);
    print(c, 100);

    realloc(c, 0);

    return 0;
}
