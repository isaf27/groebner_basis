#ifndef GROEBNER_BASIS_POLYNOMIAL_H
#define GROEBNER_BASIS_POLYNOMIAL_H

#include "monomial.h"
#include <functional>
#include <map>
#include <initializer_list>
#include <iostream>
#include <utility>

namespace polynomial {

    template<uint32_t size, class Field, class Compare = std::less<Monomial<size>>>
    class Polynomial {
    public:
        Polynomial() : terms_({}) {
        }

        Polynomial(const Monomial<size>& monomial, const Field& coefficient) {
            if (!coefficient.is_zero()) {
                terms_[monomial] = coefficient;
            }
        }

        Polynomial(std::map<Monomial<size>, Field, Compare>&& monomials) : terms_(monomials) {
        }

        Polynomial(std::initializer_list<std::pair<Monomial<size>, Field>> elements) {
            for (const auto& element : elements) {
                if (!element.second.is_zero()) {
                    terms_[element.first] += element.second;
                    if (terms_[element.first] == 0) {
                        terms_.erase(element.first);
                    }
                }
            }
        }

        bool operator==(const Polynomial& other) const {
            return terms_ == other.terms_;
        }

        bool operator!=(const Polynomial& other) const {
            return terms_ != other.terms_;
        }

        Polynomial operator+(const Polynomial& other) const {
            std::map<Monomial<size>, Field, Compare> result = terms_;
            for (const auto& monomial : other.terms_) {
                result[monomial.first] += monomial.second;
                if (result[monomial.first] == 0) {
                    result.erase(monomial.first);
                }
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator+=(const Polynomial& other) {
            for (const auto& monomial : other.terms_) {
                terms_[monomial.first] += monomial.second;
                if (terms_[monomial.first] == 0) {
                    terms_.erase(monomial.first);
                }
            }
            return *this;
        }

        Polynomial operator-(const Polynomial& other) const {
            std::map<Monomial<size>, Field, Compare> result = terms_;
            for (const auto& monomial : other.terms_) {
                result[monomial.first] -= monomial.second;
                if (result[monomial.first] == 0) {
                    result.erase(monomial.first);
                }
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator-=(const Polynomial& other) {
            for (const auto& monomial : other.terms_) {
                terms_[monomial.first] -= monomial.second;
                if (terms_[monomial.first] == 0) {
                    terms_.erase(monomial.first);
                }
            }
            return *this;
        }

        Polynomial operator*(const Field& coefficient) const {
            if (coefficient.is_zero()) {
                return Polynomial<size, Field, Compare>();
            }
            std::map<Monomial<size>, Field, Compare> result = terms_;
            for (const auto& monomial : terms_) {
                result[monomial.first] *= coefficient;
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator*=(const Field& coefficient) {
            if (coefficient.is_zero()) {
                terms_.clear();
            } else {
                for (const auto& monomial : terms_) {
                    terms_[monomial.first] *= coefficient;
                }
            }
            return *this;
        }

        Polynomial operator/(const Field& coefficient) const {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            std::map<Monomial<size>, Field, Compare> result = terms_;
            for (const auto& monomial : terms_) {
                result[monomial.first] /= coefficient;
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator/=(const Field& coefficient) {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            for (const auto& monomial : terms_) {
                terms_[monomial.first] /= coefficient;
            }
            return *this;
        }

        Polynomial operator*(const Monomial<size>& monomial) const {
            std::map<Monomial<size>, Field, Compare> result;
            for (const auto& element : terms_) {
                result[element.first * monomial] = element.second;
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator*=(const Monomial<size>& monomial) {
            *this = *this * monomial;
            return *this;
        }

        Polynomial operator*(const Polynomial& other) const {
            std::map<Monomial<size>, Field, Compare> result;
            for (const auto& first_monomial : terms_) {
                for (const auto& second_monomial : other.terms_) {
                    const auto multiply = first_monomial.first * second_monomial.first;
                    result[multiply] += first_monomial.second * second_monomial.second;
                    if (result[multiply] == 0) {
                        result.erase(multiply);
                    }
                }
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator*=(const Polynomial& other) {
            *this = *this * other;
            return *this;
        }

        void reduce(Polynomial& polynomial) {
            while (!polynomial.is_zero()) {
                const auto& major_monomial = polynomial.get_major_monomial();
                if (!major_monomial.is_subset(get_major_monomial())) {
                    break;
                }
                polynomial -= (*this) * (major_monomial / get_major_monomial()) * get_major_coefficient();
            }
        }

        friend std::ostream& operator<<(std::ostream &out, const Polynomial &element) {
            uint32_t monomials_count = element.terms_.size();
            for (const auto& monomial : element.terms_) {
                monomials_count--;
                if (monomial.first.is_empty() && monomial.second.is_one()) {
                    out << Field(1);
                } else {
                    if (!monomial.second.is_one()) {
                        out << monomial.second;
                        if (!monomial.first.is_empty()) {
                            out << "*";
                        }
                    }
                    if (!monomial.first.is_empty()) {
                        out << monomial.first;
                    }
                }
                if (monomials_count > 0) {
                    out << " + ";
                }
            }
            return out;
        }

        bool is_zero() const {
            return terms_.empty();
        }

        Monomial<size> get_major_monomial() const {
            if (is_zero()) {
                return Monomial<size>();
            } else {
                return terms_.rbegin()->first;
            }
        }

        Field get_major_coefficient() const {
            if (is_zero()) {
                return 0;
            } else {
                return terms_.rbegin()->second;
            }
        }

        Polynomial get_major() const {
            if (is_zero()) {
                return Polynomial();
            }
            return {*terms_.rbegin()};
        }

        Polynomial& operator=(Polynomial copy) {
            terms_ = copy.terms_;
            return (*this);
        }

        Polynomial(const Polynomial &copy) : terms_(copy.terms_) {
        }

    private:
        std::map<Monomial<size>, Field, Compare> terms_;
    };
}

#endif
