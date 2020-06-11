#ifndef GROEBNER_BASIS_MONOMIAL_H
#define GROEBNER_BASIS_MONOMIAL_H

#include <cassert>
#include <algorithm>
#include <vector>
#include <iostream>

namespace polynomial {

    using MonomialDegreeType = uint32_t;

    class Monomial {
    public:
        Monomial() = default;

        Monomial(std::vector<MonomialDegreeType>&& degree) : degree_(degree) {
            while (!degree_.empty() && degree_.back() == 0) {
                degree_.pop_back();
            }
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
            std::vector<MonomialDegreeType> result(std::max(first.size(), second.size()), 0);
            for (size_t i = 0; i < first.size(); ++i) {
                result[i] = first.degree_[i];
            }
            for (size_t i = 0; i < second.size(); ++i) {
                result[i] += second.degree_[i];
            }
            return std::move(result);
        }

        Monomial operator*=(const Monomial& other) {
            if (size() < other.size()) {
                degree_.resize(other.size(), 0);
            }
            for (size_t i = 0; i < other.size(); ++i) {
                degree_[i] += other.degree_[i];
            }
            return *this;
        }

        friend Monomial operator/(const Monomial& first, const Monomial& second) {
            assert(((void)"divider should be a subset of dividend", first.size() >= second.size()));
            std::vector<MonomialDegreeType> result(first.degree_.begin(), first.degree_.end());
            for (size_t i = 0; i < second.size(); ++i) {
                assert(((void)"divider should be a subset of dividend", first.degree_[i] >= second.degree_[i]));
                result[i] -= second.degree_[i];
            }
            return std::move(result);
        }

        Monomial operator/=(const Monomial& other) {
            assert(((void)"divider should be a subset of dividend", size() >= other.size()));
            for (size_t i = 0; i < other.size(); ++i) {
                assert(((void)"divider should be a subset of dividend", degree_[i] >= other.degree_[i]));
                degree_[i] -= other.degree_[i];
            }
            while (!degree_.empty() && degree_.back() == 0) {
                degree_.pop_back();
            }
            return *this;
        }

        size_t size() const {
            return degree_.size();
        }

        bool is_subset(const Monomial& other) const {
            if (size() < other.size()) {
                return false;
            }
            for (size_t i = 0; i < other.size(); ++i) {
                if (degree_[i] < other.degree_[i]) {
                    return false;
                }
            }
            return true;
        }

        bool is_empty() const {
            return degree_.empty();
        }

        MonomialDegreeType get_degree(size_t num) const {
            if (num >= size()) {
                return 0;
            }
            return degree_[num];
        }

        friend std::ostream& operator<<(std::ostream &out, const Monomial &element) {
            if (element.is_empty()) {
                out << "1";
                return out;
            }
            for (size_t i = 0; i < element.size(); ++i) {
                if (element.degree_[i] > 0) {
                    out << "x_" << i;
                    if (element.degree_[i] > 1) {
                        out << "^" << element.degree_[i];
                    }
                    if (i + 1 != element.size()) {
                        out << "*";
                    }
                }
            }
            return out;
        }

    private:
        std::vector<MonomialDegreeType> degree_;
    };

    Monomial get_intersection(const Monomial& first, const Monomial& second) {
        std::vector<MonomialDegreeType> degree(std::min(first.size(), second.size()));
        for (size_t i = 0; i < degree.size(); ++i) {
            degree[i] = std::min(first.get_degree(i), second.get_degree(i));
        }
        return std::move(degree);
    }
}

#endif
