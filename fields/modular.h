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
        Modular() = default;

        Modular(ModularValueType value) : value_(value) {
            assert(((void)"value should be less than modulo", value < modulo));
        }

        friend bool operator==(const Modular& first, const Modular& second) {
            return first.value_ == second.value_;
        }

        friend bool operator!=(const Modular& first, const Modular& second) {
            return !(first == second);
        }

        friend Modular operator+(const Modular& first, const Modular& second) {
            return (first.value_ + second.value_) % modulo;
        }

        Modular operator+=(const Modular& other) {
            value_ = (value_ + other.value_) % modulo;
            return *this;
        }

        friend Modular operator-(const Modular& first, const Modular& second) {
            return (first.value_ + modulo - second.value_) % modulo;
        }

        Modular operator-=(const Modular& other) {
            value_ = (value_ + modulo - other.value_) % modulo;
            return *this;
        }

        friend Modular operator*(const Modular& first, const Modular& second) {
            return (
                static_cast<ModularValueType64>(first.value_) *
                static_cast<ModularValueType64>(second.value_) %
                static_cast<ModularValueType64>(modulo)
            );
        }

        Modular operator*=(const Modular& other) {
            value_ = (
                static_cast<ModularValueType64>(value_) *
                static_cast<ModularValueType64>(other.value_) %
                static_cast<ModularValueType64>(modulo)
            );
            return *this;
        }

        friend Modular operator/(const Modular& first, const Modular& second) {
            return first * second.inverse();
        }

        Modular operator/=(const Modular& other) {
            *this *= other.inverse();
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
            ModularValueType degree = modulo - 2;
            Modular result = 1u;
            Modular current_power = *this;
            for (ModularValueType current_degree = 1u; current_degree <= degree; current_degree <<= 1u) {
                if (degree & current_degree) {
                    result *= current_power;
                }
                current_power *= current_power;
            }
            return result;
        }

        ModularValueType value_ = 0;
    };
}

#endif
