/*
 * boost library should be installed to use this functions
 */

#ifndef GROEBNER_BASIS_PARSER_H
#define GROEBNER_BASIS_PARSER_H

#include "../fields/rational.h"
#include "../library/polynomial.h"

#include <cassert>
#include <sstream>

namespace parser {

    using RationalType = math::Rational;
    using MonomialType = polynomial::Monomial;
    using PolynomialType = polynomial::Polynomial<RationalType>;

    RationalType parse_coefficient(const std::string& s, size_t l, size_t r) {
        std::string now = "";
        for (size_t i = l; i < r; ++i) {
            if (isdigit(s[i]) || s[i] == '/' || s[i] == '-') {
                now += s[i];
            }
        }
        std::stringstream sin(now);
        math::RationalType value;
        sin >> value;
        return value;
    }

    MonomialType parse_monomial(const std::string& s, size_t l, size_t r) {
        if (l + 2 >= r) {
            return MonomialType();
        }
        assert(((void)"variable should start with x_", s[l] == 'x' && s[l + 1] == '_'));
        std::string now = "";
        for (size_t i = l + 2; i < r; ++i) {
            if (!isdigit(s[i])) {
                l = i;
                break;
            }
            now += s[i];
        }
        now += " ";
        bool degree_found = false;
        for (size_t i = l; i < r; i++) {
            if (s[i] == '^') {
                degree_found = true;
                l = i + 1;
                break;
            }
        }
        if (degree_found) {
            for (size_t i = l; i < r; ++i) {
                if (!isdigit(s[i])) {
                    break;
                }
                now += s[i];
            }
        } else {
            now += "1";
        }
        std::stringstream sin(now);
        size_t pos;
        polynomial::MonomialDegreeType degree;
        sin >> pos >> degree;
        std::vector<polynomial::MonomialDegreeType> degrees(pos + 1, 0);
        degrees[pos] = degree;
        return MonomialType(std::move(degrees));
    }

    PolynomialType parse_atom(const std::string& s, size_t l, size_t r) {
        for (size_t i = l; i < r; ++i) {
            if (s[i] == 'x') {
                return PolynomialType(parse_monomial(s, i, r), 1);
            }
        }
        return PolynomialType({}, parse_coefficient(s, l, r));
    }

    PolynomialType parse_term(const std::string& s, size_t l, size_t r) {
        PolynomialType result({}, 1);
        size_t from = l;
        for (size_t i = l; i < r; ++i) {
            if (s[i] == '*') {
                result *= parse_atom(s, from, i);
                from = i + 1;
                continue;
            }
        }
        result *= parse_atom(s, from, r);
        return result;
    }

    PolynomialType parse_term(const std::string& s) {
        return parse_term(s, 0, s.size());
    }

    PolynomialType parse_polynomial(const std::string& s, size_t l, size_t r) {
        PolynomialType result;
        bool is_plus = true;
        size_t from = l;
        for (size_t i = l; i < r; ++i) {
            if (s[i] == '+' || s[i] == '-') {
                if (is_plus) {
                    result += parse_term(s, from, i);
                } else {
                    result -= parse_term(s, from, i);
                }
                is_plus = (s[i] == '+');
                from = i + 1;
                continue;
            }
        }
        if (is_plus) {
            result += parse_term(s, from, r);
        } else {
            result -= parse_term(s, from, r);
        }
        return result;
    }

    PolynomialType parse_polynomial(const std::string& s) {
        return parse_polynomial(s, 0, s.size());
    }
}

#endif
