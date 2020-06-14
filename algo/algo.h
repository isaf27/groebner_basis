/*
 * boost library should be installed to use this class
 */

#ifndef GROEBNER_BASIS_ALGO_H
#define GROEBNER_BASIS_ALGO_H

#include "../fields/rational.h"
#include "../library/ideal.h"

namespace algo {

    using RationalType = math::Rational;
    using PolynomialType = polynomial::Polynomial<RationalType>;
    using IdealType = polynomial::Ideal<RationalType>;

    bool solutions_existance(const std::vector<PolynomialType>& polynomials) {
        IdealType ideal;
        for (const auto& polynomial : polynomials) {
            ideal.add(polynomial);
        }
        return !ideal.is_full();
    }

    bool solutions_finiteness(const std::vector<PolynomialType>& polynomials, size_t size = 0) {
        IdealType ideal;
        for (const auto& polynomial : polynomials) {
            ideal.add(polynomial);
        }
        size_t cur_size = ideal.size();
        if (size == 0) {
            size = cur_size;
        } else {
            assert(((void)"the number of variables can't be less, than the number of variables in polynomials", size >= cur_size));
        }
        if (size > cur_size) {
            return ideal.is_full();
        }
        return ideal.are_all_powers_exist();
    }

    bool radical_contains(const PolynomialType& polynomial, IdealType ideal) {
        size_t size = ideal.size();
        std::vector<polynomial::MonomialDegreeType> degree(size + 1, 0);
        degree[size] = 1;
        PolynomialType one;
        one.add({}, 1);
        ideal.add(one - polynomial * polynomial::Monomial(std::move(degree)));
        return ideal.is_full();
    }

    bool is_systems_subset(const std::vector<PolynomialType>& subset, const std::vector<PolynomialType>& superset) {
        IdealType ideal;
        for (const auto& polynomial : subset) {
            ideal.add(polynomial);
        }
        for (const auto& polynomial : superset)
            if (!radical_contains(polynomial, ideal))
                return false;
        return true;
    }

    bool systems_equivalence(const std::vector<PolynomialType>& first, const std::vector<PolynomialType>& second) {
        return is_systems_subset(first, second) && is_systems_subset(second, first);
    }
}

#endif
