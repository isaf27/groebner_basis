#ifndef GROEBNER_BASIS_MODULAR_H
#define GROEBNER_BASIS_MODULAR_H

#include <cassert>
#include <iostream>

namespace math {

    using ModularValueType = uint32_t;
    using ModularValueType64 = uint64_t;

    template <ModularValueType modulo>
    class Modular {
    public:
        Modular(ModularValueType value = 0) : value_(value) {
            assert(((void)"value should be less than modulo", value < modulo));
        }

        bool operator==(const Modular& other) const {
            return value_ == other.value_;
        }

        bool operator!=(const Modular& other) const {
            return !(*this == other);
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
            return Modular<modulo>(
                static_cast<ModularValueType64>(value_) *
                static_cast<ModularValueType64>(other.value_) %
                static_cast<ModularValueType64>(modulo)
            );
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

    private:
        Modular power(ModularValueType degree) const {
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

        ModularValueType value_;
    };
}

#endif
