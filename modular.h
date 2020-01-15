#ifndef GROEBNER_BASIS_MODULAR_H
#define GROEBNER_BASIS_MODULAR_H

#include <cassert>
#include <iostream>

namespace math {

    template <uint32_t modulo>
    class Modular {
    public:
        Modular(uint32_t value = 0) : value_(value) {
            assert(((void)"value should be less than modulo", value < modulo));
        }

        bool operator==(const Modular& other) const {
            return value_ == other.value_;
        }

        bool operator!=(const Modular& other) const {
            return value_ != other.value_;
        }

        Modular operator+(const Modular& other) const {
            if (value_ + other.value_ >= modulo) {
                return Modular<modulo>(value_ + other.value_ - modulo);
            } else {
                return Modular<modulo>(value_ + other.value_);
            }
        }

        Modular operator+=(const Modular& other) {
            *this = *this + other;
            return *this;
        }

        Modular operator-(const Modular& other) const {
            if (value_ >= other.value_) {
                return Modular<modulo>(value_ - other.value_);
            } else {
                return Modular<modulo>(value_ + modulo - other.value_);
            }
        }

        Modular operator-=(const Modular& other) {
            *this = *this - other;
            return *this;
        }

        Modular operator*(const Modular& other) const {
            return Modular<modulo>((uint64_t)value_ * (uint64_t)other.value_ % (uint64_t)modulo);
        }

        Modular operator*=(const Modular& other) {
            *this = *this * other;
            return *this;
        }

        Modular operator/(const Modular& other) const {
            return *this * other.inverse();
        }

        Modular operator/=(const Modular& other) {
            *this = *this / other;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream &out, const Modular &element) {
            out << "[" << element.value_ << " (modulo " << modulo << ")]";
            return out;
        }

        bool is_zero() const {
            return value_ == 0;
        }

        bool is_one() const {
            return value_ == 1;
        }

        Modular& operator=(Modular copy) {
            value_ = copy.value_;
            return *this;
        }

        Modular(const Modular &copy) : value_(copy.value_) {
        }

    private:
        Modular power(uint32_t degree) const {
            if (degree == 0) {
                return Modular<modulo>(1);
            }
            const auto subpower = this->power(degree / 2);
            if (degree % 2 == 0) {
                return subpower * subpower;
            } else {
                return subpower * subpower * *this;
            }
        }

        Modular inverse() const {
            assert(((void)"division by zero", value_ != 0));
            return this->power(modulo - 2);
        }

        uint32_t value_;
    };
}

#endif
