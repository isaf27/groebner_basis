#include "../fields/rational.h"
#include "../library/ideal.h"
#include "../parser/io.h"

#include <iostream>

using namespace std;
using namespace math;
using namespace polynomial;

int main() {
    int n;
    cin >> n;
    Ideal<Rational> ideal;
    for (int i = 0; i < n; i++) {
        Polynomial<Rational> p;
        cin >> p;
        ideal.add(p);
    }
    ideal.make_minimal_groebner_basis();
    cout << ideal << endl;
    return 0;
}