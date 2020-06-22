#include "../fields/modular.h"
#include "../library/polynomial.h"

#include <iostream>

using namespace std;
using namespace math;
using namespace polynomial;

int main() {
    Polynomial<Modular<11>> a({
        make_pair(Monomial({1, 0}), 2),
        make_pair(Monomial({0, 1}), 3)
    });
    cout << a << endl;
    Polynomial<Modular<11>> b(Monomial({2, 0}), 1);
    Polynomial<Modular<11>> c(Monomial({0, 2}), 1);
    Polynomial<Modular<11>> d(Monomial({1, 1}), 1);
    cout << a * a << endl;
    cout << (a * a == b * 4 + c * 9 + d ? "true" : "false") << endl;
    a.reduce(b);
    cout << b << endl;
    return 0;
}