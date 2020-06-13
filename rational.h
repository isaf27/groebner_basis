/*
 * boost library should be installed to use this class
 */

#ifndef GROEBNER_BASIS_RATIONAL_H

#define GROEBNER_BASIS_RATIONAL_H

#include <boost/multiprecision/gmp.hpp>

#include <cassert>
#include <iostream>

namespace math {

    using RationalType = boost::multiprecision::mpq_rational;

    class Rational {
    public:
        Rational() = default;

        template <typename T>
        Rational(T value) : value_(value) {}

        friend bool operator==(const Rational& first, const Rational& second) {
            return first.value_ == second.value_;
        }

        friend bool operator!=(const Rational& first, const Rational& second) {
            return first != second;
        }

        friend Rational operator+(const Rational& first, const Rational& second) {
            return first.value_ + second.value_;
        }

        Rational operator+=(const Rational& other) {
            value_ = value_ + other.value_;
            return *this;
        }

        friend Rational operator-(const Rational& first, const Rational& second) {
            return first.value_ - second.value_;
        }

        Rational operator-=(const Rational& other) {
            value_ = value_ - other.value_;
            return *this;
        }

        friend Rational operator*(const Rational& first, const Rational& second) {
            return first.value_ * second.value_;
        }

        Rational operator*=(const Rational& other) {
            value_ = value_ * other.value_;
            return *this;
        }

        friend Rational operator/(const Rational& first, const Rational& second) {
            return first.value_ / second.value_;
        }

        Rational operator/=(const Rational& other) {
            value_ = value_ / other.value_;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream &out, const Rational &element) {
            out << element.value_;
            return out;
        }

        bool is_zero() const {
            return value_ == 0;
        }

        bool is_one() const {
            return value_ == 1;
        }

        RationalType value_ = 0;
    };
}

#endif