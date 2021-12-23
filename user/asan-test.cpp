#include <cstdlib>
#include <cstdio>

void testShadowPre() {
    int *c = (int *)malloc(10 * sizeof(*c));
    printf("%d\n", c[-1]);
}

void testShadowAft() {
    int *c = (int *)malloc(10  * sizeof(*c));
    printf("%d\n", c[10]);
}

void testReallocShadowAft() {
    int *c = (int *)malloc(10 * sizeof(c));
    c = (int *)realloc(c, 1 * sizeof(c));
    printf("%d\n", c[5]);
}

int main(int argc, char **argv) {
    // testShadowPre();
    // testShadowAft();
    // testReallocShadowAft();
    return 0;
}
