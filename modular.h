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
        Modular inverse() const {
            assert(((void)"division by zero", value_ != 0));
            uint32_t degree = modulo - 2;
            Modular result = 1u;
            Modular current_power = *this;
            for (uint32_t current_degree = 1u; current_degree <= degree; current_degree <<= 1u) {
                if (degree & current_degree) {
                    result *= current_power;
                }
                current_power *= current_power;
            }
            return result;
        }

        ModularValueType value_;
    };
}

#endif
