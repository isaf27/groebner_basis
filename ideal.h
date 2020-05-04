#ifndef GROEBNER_BASIS_IDEAL_H
#define GROEBNER_BASIS_IDEAL_H

#include "polynomial.h"

#include <vector>
#include <initializer_list>
#include <algorithm>

namespace polynomial {

    enum class BasisType {
        Any,
        Groebner,
        MinimizationGroebner,
        AutoreductionGroebner,
        UniqueGroebner
    };

    template<class Field, class Compare = std::less<Monomial>>
    class Ideal {
    public:
        Ideal() = default;

        Ideal(std::vector<Polynomial<Field, Compare>>&& polynomials) : polynomials_(polynomials) {}

        Ideal(std::initializer_list<Polynomial<Field, Compare>> polynomials) {
            for (const auto& polynomial : polynomials) {
                if (!polynomial.is_zero()) {
                    polynomials_.push_back(polynomial);
                    polynomials_.back() /= polynomials_.back().get_major_coefficient();
                }
            }
        }

        void add(const Polynomial<Field, Compare>& polynomial) {
            type_ = BasisType::Any;
            if (!polynomial.is_zero()) {
                polynomials_.push_back(polynomial);
                polynomials_.back() /= polynomials_.back().get_major_coefficient();
            }
        }

        void reduce(Polynomial<Field, Compare>& polynomial) const {
            while (!polynomial.is_zero()) {
                const auto major_monomial = polynomial.get_major_monomial();
                bool was_reduced = false;
                for (const auto& reducer : polynomials_) {
                    if (!major_monomial.is_subset(reducer.get_major_monomial())) {
                        continue;
                    }
                    was_reduced = true;
                    polynomial -= reducer * (major_monomial / reducer.get_major_monomial()) * polynomial.get_major_coefficient();
                    break;
                }
                if (!was_reduced) {
                    break;
                }
            }
        }

        void full_reduce(Polynomial<Field, Compare>& polynomial) const {
            while (!polynomial.is_zero()) {
                bool was_reduced = false;
                for (const auto& reducer : polynomials_) {
                    const auto div = polynomial / reducer.get_major_monomial();
                    if (div.is_zero()) {
                        continue;
                    }
                    was_reduced = true;
                    polynomial -= reducer * div;
                }
                if (!was_reduced) {
                    break;
                }
            }
        }

        void make_groebner_basis() {
            if (type_ != BasisType::Any) {
                return;
            }
            type_ = BasisType::Groebner;
            for (size_t i = 0; i < polynomials_.size(); ++i) {
                for (size_t j = 0; j < i; ++j) {
                    const auto intersection = get_intersection(
                        polynomials_[i].get_major_monomial(),
                        polynomials_[j].get_major_monomial()
                    );
                    if (intersection.is_empty()) {
                        continue;
                    }
                    auto s_polynomial = polynomials_[i] * (polynomials_[j].get_major_monomial() / intersection);
                    s_polynomial -= polynomials_[j] * (polynomials_[i].get_major_monomial() / intersection);
                    reduce(s_polynomial);
                    add(s_polynomial);
                }
            }
        }

        void make_minimization() {
            if (type_ == BasisType::MinimizationGroebner || type_ == BasisType::UniqueGroebner) {
                return;
            }
            type_ = BasisType::MinimizationGroebner;
            size_t i = 0;
            while (i < polynomials_.size()) {
                const auto& major_monomial = polynomials_[i].get_major_monomial();
                bool remove_needed = false;
                for (size_t j = 0; j < polynomials_.size(); ++j) {
                    if (i != j) {
                        if (major_monomial.is_subset(polynomials_[j].get_major_monomial())) {
                            remove_needed = true;
                            break;
                        }
                    }
                }
                if (remove_needed) {
                    std::swap(polynomials_[i], polynomials_.back());
                    polynomials_.pop_back();
                } else {
                    i++;
                }
            }
        }

        void make_autoreduction() {
            if (type_ == BasisType::AutoreductionGroebner || type_ == BasisType::UniqueGroebner) {
                return;
            }
            type_ = BasisType::AutoreductionGroebner;
            for (size_t i = 0; i < polynomials_.size(); ++i) {
                auto remainder = polynomials_[i] - polynomials_[i].get_major_term();
                polynomials_[i] = polynomials_[i].get_major_term();
                full_reduce(remainder);
                polynomials_[i] += remainder;
            }
        }

        void make_minimal_groebner_basis() {
            if (type_ == BasisType::UniqueGroebner) {
                return;
            }
            make_groebner_basis();
            make_minimization();
            make_autoreduction();
            type_ = BasisType::UniqueGroebner;
            Compare cmp;
            std::sort(
                polynomials_.begin(),
                polynomials_.end(),
                [cmp] (const Polynomial<Field, Compare>& left, const Polynomial<Field, Compare>& right) {
                    return cmp(left.get_major_monomial(), right.get_major_monomial());
                }
            );
        }

        friend std::ostream& operator<<(std::ostream &out, const Ideal &ideal) {
            out << "{";
            bool is_comma_needed = false;
            for (const auto& polynomial : ideal.polynomials_) {
                if (is_comma_needed) {
                    out << ", ";
                }
                is_comma_needed = true;
                out << polynomial;
            }
            out << "}";
            return out;
        }

        bool is_empty() const {
            return polynomials_.empty();
        }

    private:
        std::vector<Polynomial<Field, Compare>> polynomials_;
        BasisType type_ = BasisType::Any;
    };
}

#endif
