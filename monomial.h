#ifndef GROEBNER_BASIS_MONOMIAL_H
#define GROEBNER_BASIS_MONOMIAL_H

#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>

namespace polynomial {

    using PolynomialDegreeType = uint32_t;

    template<PolynomialDegreeType size>
    class Monomial {
    public:
        Monomial() : degree_(size, 0) {
        }

        Monomial(std::vector<PolynomialDegreeType> degree) : degree_(degree) {
        }

        Monomial operator*(const Monomial& other) const {
            std::vector<PolynomialDegreeType> result(size, 0);
            for (PolynomialDegreeType i = 0; i < size; ++i) {
                result[i] = degree_[i] + other.degree_[i];
            }
            return Monomial<size>(result);
        }

        Monomial operator/(const Monomial& other) const {
            std::vector<PolynomialDegreeType> result(size, 0);
            for (PolynomialDegreeType i = 0; i < size; ++i) {
                assert(((void)"divider should be a subset of dividend", degree_[i] >= other.degree_[i]));
                result[i] = degree_[i] - other.degree_[i];
            }
            return Monomial<size>(result);
        }

        bool operator==(const Monomial& other) const {
            return degree_ == other.degree_;
        }

        bool operator!=(const Monomial& other) const {
            return !(*this == other);
        }

        bool operator<(const Monomial& other) const {
            return degree_ < other.degree_;
        }

        bool is_subset(const Monomial& other) const {
            for (PolynomialDegreeType i = 0; i < size; ++i) {
                if (degree_[i] < other.degree_[i]) {
                    return false;
                }
            }
            return true;
        }

        bool is_empty() const {
            for (PolynomialDegreeType i = 0; i < size; ++i) {
                if (degree_[i] != 0) {
                    return false;
                }
            }
            return true;
        }

        PolynomialDegreeType get_degree(PolynomialDegreeType num) const {
            assert(((void)"degree should be between 0 and size - 1", (size > num && num >= 0)));
            return degree_[num];
        }

        friend std::ostream& operator<<(std::ostream &out, const Monomial &element) {
            PolynomialDegreeType non_zero_count = 0;
            for (PolynomialDegreeType i = 0; i < size; ++i) {
                non_zero_count += element.degree_[i] > 0;
            }
            if (non_zero_count == 0) {
                out << "1";
            } else {
                for (PolynomialDegreeType i = 0; i < size; ++i) {
                    if (element.degree_[i] > 0) {
                        out << "x_" << i + 1;
                        if (element.degree_[i] > 1) {
                            out << "^" << element.degree_[i];
                        }
                        non_zero_count--;
                        if (non_zero_count > 0) {
                            out << "*";
                        }
                    }
                }
            }
            return out;
        }

        Monomial& operator=(Monomial copy) {
            degree_ = copy.degree_;
            return *this;
        }

        Monomial(const Monomial &copy) : degree_(copy.degree_) {
        }

    private:
        std::vector<PolynomialDegreeType> degree_;
    };

    template<PolynomialDegreeType size>
    Monomial<size> get_intersection(const Monomial<size>& first, const Monomial<size>& second) {
        std::vector<PolynomialDegreeType> degree(size);
        for (PolynomialDegreeType i = 0; i < size; ++i) {
            degree[i] = std::min(first.get_degree(i), second.get_degree(i));
        }
        return Monomial<size>(degree);
    }
}

#endif
