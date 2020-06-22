#include "../fields/modular.h"
#include "../library/ideal.h"

#include <iostream>

using namespace std;
using namespace math;
using namespace polynomial;

int main() {
    Polynomial<Modular<19>> f1({
        {Monomial({2, 0, 0}), 1},
        {Monomial({0, 1, 0}), 18}
    });
    Polynomial<Modular<19>> f2({
        {Monomial({2, 0, 0}), 1},
        {Monomial({0, 0, 1}), 18}
    });
    Polynomial<Modular<19>> one(Monomial({0, 0, 0}), 1);
    Ideal<Modular<19>> i({f1, f2});
    cout << i << endl;
    i.make_groebner_basis();
    cout << i << endl;
    i.make_minimal_groebner_basis();
    cout << i << endl;
    Polynomial<Modular<19>> f3 = f1 * Monomial({1, 2, 3}) + f2 * f2 * 5 + one;
    i.reduce(f3);
    cout << f3 << endl;
    return 0;
}