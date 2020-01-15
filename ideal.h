#ifndef GROEBNER_BASIS_IDEAL_H
#define GROEBNER_BASIS_IDEAL_H

#include "polynomial.h"
#include <vector>

namespace polynomial {

    template<uint32_t size, class Field, class Compare = std::less<Monomial<size>>>
    class Ideal {
    public:
        Ideal() : polynomials_({}) {
        }

        Ideal(const std::vector<Polynomial<size, Field, Compare>>& elements) {
            for (const auto& polynomial : elements) {
                if (!polynomial.is_zero()) {
                    polynomials_.push_back(polynomial);
                    polynomials_.back() /= polynomials_.back().get_major_coefficient();
                }
            }
        }

        void add(const Polynomial<size, Field, Compare>& polynomial) {
            if (!polynomial.is_zero()) {
                polynomials_.push_back(polynomial);
                polynomials_.back() /= polynomials_.back().get_major_coefficient();
            }
        }

        void reduce(Polynomial<size, Field, Compare>& polynomial) {
            while (!polynomial.is_zero()) {
                const auto& major_monomial = polynomial.get_major_monomial();
                bool was_reduced = false;
                for (const auto& reducer : polynomials_) {
                    if (major_monomial.is_subset(reducer.get_major_monomial())) {
                        polynomial -= reducer * (major_monomial / reducer.get_major_monomial()) * polynomial.get_major_coefficient();
                        was_reduced = true;
                        break;
                    }
                }
                if (!was_reduced) {
                    break;
                }
            }
        }

        void make_groebner_basis() {
            for (uint32_t i = 0; i < polynomials_.size(); ++i) {
                for (uint32_t j = 0; j < i; ++j) {
                    const auto& intersection = get_intersection(
                            polynomials_[i].get_major_monomial(),
                            polynomials_[j].get_major_monomial()
                    );
                    if (intersection.is_empty()) {
                        continue;
                    }
                    auto s_polynomial = polynomials_[i] * (polynomials_[j].get_major_monomial() / intersection);
                    s_polynomial -= polynomials_[j] * (polynomials_[i].get_major_monomial() / intersection);
                    this->reduce(s_polynomial);
                    this->add(s_polynomial);
                }
            }
        }

        void make_minimization() {
            uint32_t i = 0;
            while (i < polynomials_.size()) {
                const auto& major_monomial = polynomials_[i].get_major_monomial();
                bool remove_needed = false;
                for (uint32_t j = 0; j < polynomials_.size() && !remove_needed; ++j) {
                    if (i != j) {
                        const auto& smaller_monomial = polynomials_[j].get_major_monomial();
                        if (major_monomial.is_subset(smaller_monomial)) {
                            if (major_monomial == smaller_monomial) {
                                remove_needed = i < j;
                            } else {
                                remove_needed = true;
                            }
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
            while (true) {
                bool was_reduced = false;
                for (uint32_t i = 0; i < polynomials_.size(); ++i) {
                    auto remainder = polynomials_[i] - polynomials_[i].get_major();
                    const auto remainder_before = remainder;
                    this->reduce(remainder);
                    if (remainder != remainder_before) {
                        polynomials_[i] -= remainder_before;
                        polynomials_[i] += remainder;
                        was_reduced = true;
                    }
                }
                if (!was_reduced) {
                    break;
                }
            }
        }

        void make_minimal_groebner_basis() {
            make_groebner_basis();
            make_minimization();
            make_autoreduction();
        }

        friend std::ostream& operator<<(std::ostream &out, const Ideal &ideal) {
            out << "(";
            uint32_t polynomials_count = ideal.polynomials_.size();
            for (const auto& polynomial : ideal.polynomials_) {
                out << polynomial;
                polynomials_count--;
                if (polynomials_count > 0) {
                    out << ", ";
                }
            }
            out << ")";
            return out;
        }

        Ideal& operator=(Ideal copy) {
            polynomials_ = copy.polynomials_;
            return *this;
        }

        Ideal(const Ideal &copy) : polynomials_(copy.polynomials_) {
        }

    private:
        std::vector<Polynomial<size, Field, Compare>> polynomials_;
    };
}

#endif
