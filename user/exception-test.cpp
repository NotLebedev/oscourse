#include <cstdio>
#include <exception>

class DivisionByZeroException : public std::exception {
public:
    ~DivisionByZeroException() {};
    const char* what() const noexcept {
    	return "Division by zero happened!";
    }
};

class FallthroughException : public std::exception {
public:
    ~FallthroughException() {};
    const char* what() const noexcept {
    	return "Exception fallthrough happened!";
    }
};

int division(int a, int b) {
   if( b == 0 ) {
      throw DivisionByZeroException{};
   }
   return a / b;
}

int main (int argc, char **argv) {
    int x = 50;
    int y = 0;
    int z = 0;

    try {
        z = division(x, y);
        printf("%d\n", z);
    } catch (std::exception& e) {
        printf("%s\n", e.what());
    }
    throw FallthroughException{};
    return 0;
}
