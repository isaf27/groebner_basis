#ifndef GROEBNER_BASIS_POLYNOMIAL_H
#define GROEBNER_BASIS_POLYNOMIAL_H

#include "monomial.h"
#include <functional>
#include <map>
#include <initializer_list>
#include <iostream>
#include <utility>

namespace polynomial {

    template<class Field, class Compare = std::less<Monomial>>
    class Polynomial {
    public:
        Polynomial() = default;

        Polynomial(const Monomial& monomial, const Field& coefficient) {
            if (!coefficient.is_zero()) {
                terms_[monomial] = coefficient;
            }
        }

        Polynomial(std::map<Monomial, Field, Compare>&& terms) : terms_(terms) {}

        Polynomial(std::initializer_list<std::pair<Monomial, Field>> elements) {
            for (const auto& element : elements) {
                if (!element.second.is_zero()) {
                    terms_[element.first] += element.second;
                    if (terms_[element.first].is_zero()) {
                        terms_.erase(element.first);
                    }
                }
            }
        }

        friend bool operator==(const Polynomial& first, const Polynomial& second) {
            return first.terms_ == second.terms_;
        }

        friend bool operator!=(const Polynomial& first, const Polynomial& second) {
            return !(first == second);
        }

        friend Polynomial operator+(const Polynomial& first, const Polynomial& second) {
            std::map<Monomial, Field, Compare> result(first.terms_);
            for (const auto& monomial : second.terms_) {
                result[monomial.first] += monomial.second;
                if (result[monomial.first].is_zero()) {
                    result.erase(monomial.first);
                }
            }
            return std::move(result);
        }

        Polynomial operator+=(const Polynomial& other) {
            for (const auto& monomial : other.terms_) {
                terms_[monomial.first] += monomial.second;
                if (terms_[monomial.first].is_zero()) {
                    terms_.erase(monomial.first);
                }
            }
            return *this;
        }

        friend Polynomial operator-(const Polynomial& first, const Polynomial& second) {
            std::map<Monomial, Field, Compare> result(first.terms_);
            for (const auto& monomial : second.terms_) {
                result[monomial.first] -= monomial.second;
                if (result[monomial.first].is_zero()) {
                    result.erase(monomial.first);
                }
            }
            return std::move(result);
        }

        Polynomial operator-=(const Polynomial& other) {
            for (const auto& monomial : other.terms_) {
                terms_[monomial.first] -= monomial.second;
                if (terms_[monomial.first].is_zero()) {
                    terms_.erase(monomial.first);
                }
            }
            return *this;
        }

        friend Polynomial operator*(const Polynomial& polynomial, const Field& coefficient) {
            if (coefficient.is_zero()) {
                return Polynomial<Field, Compare>();
            }
            std::map<Monomial, Field, Compare> result(polynomial.terms_);
            for (auto& monomial : result) {
                monomial.second *= coefficient;
            }
            return std::move(result);
        }

        Polynomial operator*=(const Field& coefficient) {
            if (coefficient.is_zero()) {
                terms_.clear();
            } else {
                for (auto& monomial : terms_) {
                    monomial.second *= coefficient;
                }
            }
            return *this;
        }

        friend Polynomial operator/(const Polynomial& polynomial, const Field& coefficient) {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            std::map<Monomial, Field, Compare> result(polynomial.terms_);
            for (auto& monomial : result) {
                monomial.second /= coefficient;
            }
            return std::move(result);
        }

        Polynomial operator/=(const Field& coefficient) {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            for (auto& monomial : terms_) {
                monomial.second /= coefficient;
            }
            return *this;
        }

        friend Polynomial operator*(const Polynomial& polynomial, const Monomial& monomial) {
            std::map<Monomial, Field, Compare> result;
            for (const auto& element : polynomial.terms_) {
                result[element.first * monomial] = element.second;
            }
            return std::move(result);
        }

        Polynomial operator*=(const Monomial& monomial) {
            *this = *this * monomial;
            return *this;
        }

        friend Polynomial operator*(const Polynomial& first, const Polynomial& second) {
            std::map<Monomial, Field, Compare> result;
            for (const auto& first_monomial : first.terms_) {
                for (const auto& second_monomial : second.terms_) {
                    const auto multiply = first_monomial.first * second_monomial.first;
                    result[multiply] += first_monomial.second * second_monomial.second;
                    if (result[multiply].is_zero()) {
                        result.erase(multiply);
                    }
                }
            }
            return std::move(result);
        }

        Polynomial operator*=(const Polynomial& other) {
            *this = *this * other;
            return *this;
        }

        void reduce(Polynomial& polynomial) const {
            assert(((void)"the reducing polynomial is a zero polynomial", !is_zero()));
            while (!polynomial.is_zero()) {
                const auto& major_monomial = polynomial.get_major_monomial();
                if (!major_monomial.is_subset(get_major_monomial())) {
                    break;
                }
                const auto coefficient = polynomial.get_major_coefficient() / get_major_coefficient();
                polynomial -= (*this) * (major_monomial / get_major_monomial()) * coefficient;
            }
        }

        friend std::ostream& operator<<(std::ostream &out, const Polynomial &polynomial) {
            for (auto monomial_iterator = polynomial.terms_.rbegin();
                 monomial_iterator != polynomial.terms_.rend();
                 ++monomial_iterator) {
                if (monomial_iterator != polynomial.terms_.rbegin()) {
                    out << "+";
                }
                const auto& monomial = monomial_iterator->first;
                const auto& coefficient = monomial_iterator->second;
                if (monomial.is_empty()) {
                    out << coefficient;
                } else {
                    if (!coefficient.is_one()) {
                        out << coefficient << "*";
                    }
                    out << monomial;
                }
            }
            return out;
        }

        bool is_zero() const {
            return terms_.empty();
        }

        Monomial get_major_monomial() const {
            assert(((void)"the polynomial is a zero polynomial", !is_zero()));
            return terms_.rbegin()->first;
        }

        Field get_major_coefficient() const {
            assert(((void)"the polynomial is a zero polynomial", !is_zero()));
            return terms_.rbegin()->second;
        }

        Polynomial get_major_term() const {
            assert(((void)"the polynomial is a zero polynomial", !is_zero()));
            return {*terms_.rbegin()};
        }

    private:
        std::map<Monomial, Field, Compare> terms_;
    };
}

#endif
