#include <cstdio>
#include <typeinfo>

class BaseClass {
public:
    virtual ~BaseClass() {};
    virtual void f() {}; // BaseClass полиморфный
};
class Derived1: public BaseClass {
public:
    ~Derived1() override {};
    void f() override {}
};

class Derived2: public BaseClass {
public:
    ~Derived2() override {};
    void f() override {}
};

enum E {
    A, B, C, D
};

int main(int argc, char **argv)
{
    int i;
    long double j;
    bool b;
    unsigned long long ull;
    BaseClass *p, baseob;
    Derived1 ob1;
    Derived2 ob2;

    E e;

    printf("Typeid of i is %s\n", typeid(i).name());
    printf("Typeid of j is %s\n", typeid(j).name());
    printf("Typeid of b is %s\n", typeid(b).name());
    printf("Typeid of ull is %s\n", typeid(ull).name());

    printf("Typeid of e is %s\n", typeid(e).name());

    printf("Typeid of main is %s\n", typeid(main).name());

    p = &baseob;
    printf("p is pointing to an object of type %s\n", typeid(*p).name());

    p = &ob1;
    printf("p is pointing to an object of type %s\n", typeid(*p).name());

    p = &ob2;
    printf("p is pointing to an object of type %s\n", typeid(*p).name());

    const BaseClass *c = new Derived1{};

    if (dynamic_cast<const Derived1*>(c) != nullptr)
        printf("correct dynamic_cast ok\n");

    if (dynamic_cast<const Derived2*>(c) == nullptr)
        printf("wrong dynamic_cast ok\n");

    delete c;
    return 0;
}
