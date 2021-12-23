#include <cstdlib>
#include <cstdio>

int *fill(int size) {
    int *c = (int *) malloc(size * sizeof(*c));
    if (c == nullptr)
        return nullptr;

    for (int i = 0; i < size; i++)
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

    free(c);

    c = fill(10000000);
    if (c == nullptr) {
        printf("Out of memory!\n");
        std::abort();
    }
    print(c, 10000000);

    free(c);


    return 0;
}
