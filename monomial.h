#ifndef GROEBNER_BASIS_MONOMIAL_H
#define GROEBNER_BASIS_MONOMIAL_H

#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>

namespace polynomial {

    using MonomialDegreeType = uint32_t;

    template<MonomialDegreeType size>
    class Monomial {
    public:
        Monomial() : degree_(size, 0) {
        }

        Monomial(std::vector<MonomialDegreeType> degree) : degree_(degree) {
        }

        friend bool operator==(const Monomial& first, const Monomial& second) {
            return first.degree_ == second.degree_;
        }

        friend bool operator!=(const Monomial& first, const Monomial& second) {
            return !(first == second);
        }

        friend bool operator<(const Monomial& first, const Monomial& second) {
            return first.degree_ < second.degree_;
        }

        friend Monomial operator*(const Monomial& first, const Monomial& second) {
            std::vector<MonomialDegreeType> result(size, 0);
            for (MonomialDegreeType i = 0; i < size; ++i) {
                result[i] = first.degree_[i] + second.degree_[i];
            }
            return result;
        }

        Monomial operator*=(const Monomial& other) {
            for (MonomialDegreeType i = 0; i < size; ++i) {
                degree_[i] += other.degree_[i];
            }
            return *this;
        }

        friend Monomial operator/(const Monomial& first, const Monomial& second) {
            std::vector<MonomialDegreeType> result(size, 0);
            for (MonomialDegreeType i = 0; i < size; ++i) {
                assert(((void)"divider should be a subset of dividend", first.degree_[i] >= second.degree_[i]));
                result[i] = first.degree_[i] - second.degree_[i];
            }
            return result;
        }

        Monomial operator/=(const Monomial& other) {
            for (MonomialDegreeType i = 0; i < size; ++i) {
                assert(((void)"divider should be a subset of dividend", degree_[i] >= other.degree_[i]));
                degree_[i] -= other.degree_[i];
            }
            return *this;
        }

        bool is_subset(const Monomial& other) const {
            for (MonomialDegreeType i = 0; i < size; ++i) {
                if (degree_[i] < other.degree_[i]) {
                    return false;
                }
            }
            return true;
        }

        bool is_empty() const {
            for (MonomialDegreeType i = 0; i < size; ++i) {
                if (degree_[i] != 0) {
                    return false;
                }
            }
            return true;
        }

        MonomialDegreeType get_degree(MonomialDegreeType num) const {
            assert(((void)"degree should be between 0 and size - 1", (size > num && num >= 0)));
            return degree_[num];
        }

        friend std::ostream& operator<<(std::ostream &out, const Monomial &element) {
            MonomialDegreeType non_zero_count = 0;
            for (MonomialDegreeType i = 0; i < size; ++i) {
                non_zero_count += element.degree_[i] > 0;
            }
            if (non_zero_count == 0) {
                out << "1";
            } else {
                for (MonomialDegreeType i = 0; i < size; ++i) {
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

    private:
        std::vector<MonomialDegreeType> degree_;
    };

    template<MonomialDegreeType size>
    Monomial<size> get_intersection(const Monomial<size>& first, const Monomial<size>& second) {
        std::vector<MonomialDegreeType> degree(size);
        for (MonomialDegreeType i = 0; i < size; ++i) {
            degree[i] = std::min(first.get_degree(i), second.get_degree(i));
        }
        return Monomial<size>(degree);
    }
}

#endif
