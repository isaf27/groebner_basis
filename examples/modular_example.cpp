#include "../fields/modular.h"

#include <iostream>

using namespace std;
using namespace math;

int main() {
    Modular<239> a = 21;
    Modular<239> b = 50;
    Modular<239> c = 31;
    cout << a + b << endl;
    cout << 200 / c << endl;
    cout << ((c - a) / b) * 2 << endl;
    cout << (a + b == b / c ? "true" : "false") << endl;
    return 0;
}