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
C c0_0{2};

void bar() {
    static C c1{5};
}


void foo() {
    static C c1{4};

    bar();
}
int
main(int argc, char **argv) {
    printf("Hello, world!\n");

    C c3{3};
    foo();
    
    return 0;
}
