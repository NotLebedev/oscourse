#include <cstdio>

#include "lib-cxx/atexit.h"
void foo();

class C {
public:
    C(int j) {i = j;printf("ctor = %d\n", i);}
    ~C() noexcept {printf("dtor = %d\n", i);}
    int get() {return i;}
private:
    int i;
};

C c0{1};
C c0_0{10};

void foo() {
    static C c1{3};
    printf("static = %d\n", c1.get());
}

int
main(int argc, char **argv) {
    printf("Hello, world!\n");

    C c3{0};
    printf("local = %d\n", c3.get());
    printf("global = %d\n", c0.get());
    foo();
    
    return 0;
}
