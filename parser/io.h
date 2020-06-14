/*
 * boost library should be installed to use this functions
 */

#ifndef GROEBNER_BASIS_IO_H
#define GROEBNER_BASIS_IO_H

#include "parser.h"

#include <istream>

std::istream& operator>>(std::istream& in, parser::PolynomialType& polynomial) {
    std::string polynomial_str;
    in >> polynomial_str;
    polynomial = parser::parse_polynomial(polynomial_str);
    return in;
}

#endif
