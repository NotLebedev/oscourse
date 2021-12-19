#include <cstdlib>
#include <cstdio>
#include <new>

int *fill(int size) {
    int *c = new int[size];

    for (int i = 0; i < size; i++)
        c[i] = i;

    return c;
}

void print(int *c, int size) {
    printf("[]");
    for (int i = 0; i < size; i++)
        printf("%d, ", c[i]);
    printf("]\n");
}

void hdlr() {
    cprintf("Out of memory!\n");
    std::abort();
}

int main(int argc, char **argv) {
    int *c = fill(10);
    print(c, 10);

    delete[] c;

    std::set_new_handler(hdlr);

    c = fill(100);
    print(c, 100);

    delete[] c;

    c = fill(10000000);
    print(c, 10000000);

    delete[] c;

    return 0;
}
