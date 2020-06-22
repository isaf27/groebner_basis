#include "../fields/rational.h"
#include "../library/ideal.h"
#include "../parser/parser.h"
#include "../algo/algo.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace math;
using namespace polynomial;
using namespace parser;
using namespace algo;

int main() {
    vector<Polynomial<Rational>> P({
        parse_polynomial("x_0*x_1-x_2^2-x_2"),
        parse_polynomial("x_0*x_2-x_1^2-x_1"),
        parse_polynomial("x_1*x_2-x_0^2-x_0")
    });
    vector<Polynomial<Rational>> Q({
        parse_polynomial("x_0*x_1-x_2^2-x_2"),
        parse_polynomial("x_1*x_2-x_0^2-x_0"),
        parse_polynomial("x_0*x_2+x_1*x_2+x_2^2+x_2"),
        parse_polynomial("x_1^2+x_1+x_1*x_2+x_2^2+x_2")
    });
    cout << systems_equivalence(P, Q) << endl;
    return 0;
}