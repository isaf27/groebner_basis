#ifndef GROEBNER_BASIS_POLYNOMIAL_H
#define GROEBNER_BASIS_POLYNOMIAL_H

#include "monomial.h"

#include <algorithm>
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

        Polynomial(std::initializer_list<std::pair<Monomial, Field>> terms) {
            for (const auto& term : terms) {
                if (!term.second.is_zero()) {
                    terms_[term.first] += term.second;
                    if (terms_[term.first].is_zero()) {
                        terms_.erase(term.first);
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
            for (const auto& term : second.terms_) {
                result[term.first] += term.second;
                if (result[term.first].is_zero()) {
                    result.erase(term.first);
                }
            }
            return std::move(result);
        }

        Polynomial operator+=(const Polynomial& other) {
            for (const auto& term : other.terms_) {
                terms_[term.first] += term.second;
                if (terms_[term.first].is_zero()) {
                    terms_.erase(term.first);
                }
            }
            return *this;
        }

        friend Polynomial operator-(const Polynomial& first, const Polynomial& second) {
            std::map<Monomial, Field, Compare> result(first.terms_);
            for (const auto& term : second.terms_) {
                result[term.first] -= term.second;
                if (result[term.first].is_zero()) {
                    result.erase(term.first);
                }
            }
            return std::move(result);
        }

        Polynomial operator-=(const Polynomial& other) {
            for (const auto& term : other.terms_) {
                terms_[term.first] -= term.second;
                if (terms_[term.first].is_zero()) {
                    terms_.erase(term.first);
                }
            }
            return *this;
        }

        friend Polynomial operator*(const Polynomial& polynomial, const Field& coefficient) {
            if (coefficient.is_zero()) {
                return Polynomial<Field, Compare>();
            }
            std::map<Monomial, Field, Compare> result(polynomial.terms_);
            for (auto& term : result) {
                term.second *= coefficient;
            }
            return std::move(result);
        }

        Polynomial operator*=(const Field& coefficient) {
            if (coefficient.is_zero()) {
                terms_.clear();
            } else {
                for (auto& term : terms_) {
                    term.second *= coefficient;
                }
            }
            return *this;
        }

        friend Polynomial operator/(const Polynomial& polynomial, const Field& coefficient) {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            std::map<Monomial, Field, Compare> result(polynomial.terms_);
            for (auto& term : result) {
                term.second /= coefficient;
            }
            return std::move(result);
        }

        Polynomial operator/=(const Field& coefficient) {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            for (auto& term : terms_) {
                term.second /= coefficient;
            }
            return *this;
        }

        friend Polynomial operator/(const Polynomial& polynomial, const Monomial& monomial) {
            Polynomial<Field, Compare> result;
            for (const auto& term : polynomial.terms_) {
                if (term.first.is_subset(monomial)) {
                    result.add(term.first / monomial, term.second);
                }
            }
            return result;
        }

        friend Polynomial operator*(const Polynomial& polynomial, const Monomial& monomial) {
            std::map<Monomial, Field, Compare> result;
            for (const auto& term : polynomial.terms_) {
                result[term.first * monomial] = term.second;
            }
            return std::move(result);
        }

        Polynomial operator*=(const Monomial& monomial) {
            *this = *this * monomial;
            return *this;
        }

        friend Polynomial operator*(const Polynomial& first, const Polynomial& second) {
            std::map<Monomial, Field, Compare> result;
            for (const auto& first_term : first.terms_) {
                for (const auto& second_term : second.terms_) {
                    const auto multiply = first_term.first * second_term.first;
                    result[multiply] += first_term.second * second_term.second;
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
                auto major_monomial = polynomial.get_major_monomial();
                if (!major_monomial.is_subset(get_major_monomial())) {
                    break;
                }
                const auto coefficient = polynomial.get_major_coefficient() / get_major_coefficient();
                major_monomial /= get_major_monomial();
                for (const auto& term : terms_) {
                    polynomial.subtract(term.first * major_monomial, term.second * coefficient);
                }
            }
        }

        void full_reduce(Polynomial& polynomial) const {
            assert(((void)"the reducing polynomial is a zero polynomial", !is_zero()));
            while (!polynomial.is_zero()) {
                auto div = polynomial / get_major_monomial();
                if (div.is_zero()) {
                    break;
                }
                div /= get_major_coefficient();
                polynomial -= (*this) * div;
            }
        }

        friend std::ostream& operator<<(std::ostream& out, const Polynomial& polynomial) {
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

        bool is_constant() const {
            if (terms_.empty()) {
                return true;
            }
            if (terms_.size() > 1u) {
                return false;
            }
            return terms_.begin()->first.is_empty();
        }

        size_t size() const {
            size_t result = 0;
            for (const auto& term : terms_) {
                result = std::max(result, term.first.size());
            }
            return result;
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

        void add(const Monomial& monomial, const Field& coefficient) {
            terms_[monomial] += coefficient;
            if (terms_[monomial].is_zero()) {
                terms_.erase(monomial);
            }
        }

        void subtract(const Monomial& monomial, const Field& coefficient) {
            terms_[monomial] -= coefficient;
            if (terms_[monomial].is_zero()) {
                terms_.erase(monomial);
            }
        }

    private:
        std::map<Monomial, Field, Compare> terms_;
    };
}

#endif
