#include "../library/monomial.h"

#include <iostream>

using namespace std;
using namespace polynomial;

int main() {
    Monomial a;
    cout << a << endl;
    Monomial b({0, 1, 0, 2, 0, 3, 1});
    cout << b << endl;
    Monomial c({0, 0, 0, 1, 0, 0, 0});
    cout << c << endl;
    Monomial d({1000});
    cout << d << endl;
    cout << b / (c * c) << endl;
    cout << b.get_degree(5) << endl;
    cout << get_intersection(b, c * c * c * d) << endl;
    return 0;
}