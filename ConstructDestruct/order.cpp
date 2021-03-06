#include <iostream>

class test {
public:
    test(int a) {
        A = a;
        std::cout << "Hey, " << a << "\n";
    }
    ~test() {
        std::cout << "Bye, " << A << "\n";
    }
    int A;
};

int main() {
    test a(1), b(2);
}