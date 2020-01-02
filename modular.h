#include <cassert>
#include <iostream>

namespace math {

    template <uint32_t modulo>
    class Modular {
    private:
        uint32_t value;

        Modular power(uint32_t degree) const {
            if (degree == 0) {
                return Modular<modulo>(1);
            }
            const auto subpower = this->power(degree / 2);
            if (degree % 2 == 0) {
                return subpower * subpower;
            } else {
                return subpower * subpower * (*this);
            }
        }

        Modular inverse() const {
            assert(((void)"division by zero", value != 0));
            return this->power(modulo - 2);
        }

    public:
        Modular(uint32_t value = 0) : value(value) {
            assert(((void)"value should be less than modulo", value < modulo));
        }

        bool operator==(const Modular& other) const {
            return value == other.value;
        }

        bool operator!=(const Modular& other) const {
            return value == other.value;
        }

        Modular operator+(const Modular& other) const {
            if (value + other.value >= modulo) {
                return Modular<modulo>(value + other.value - modulo);
            } else {
                return Modular<modulo>(value + other.value);
            }
        }

        Modular operator+=(const Modular& other) {
            *this = *this + other;
            return *this;
        }

        Modular operator-(const Modular& other) const {
            if (value >= other.value) {
                return Modular<modulo>(value - other.value);
            } else {
                return Modular<modulo>(value + modulo - other.value);
            }
        }

        Modular operator-=(const Modular& other) {
            *this = *this - other;
            return *this;
        }

        Modular operator*(const Modular& other) const {
            return Modular<modulo>((uint64_t)value * (uint64_t)other.value % (uint64_t)modulo);
        }

        Modular operator*=(const Modular& other) {
            *this = *this * other;
            return *this;
        }

        Modular operator/(const Modular& other) const {
            return (*this) * other.inverse();
        }

        Modular operator/=(const Modular& other) {
            *this = *this / other;
            return *this;
        }

        friend std::ostream& operator<<(std::ostream &out, const Modular &element) {
            out << "[" << element.value << " (modulo " << modulo << ")]";
            return out;
        }

        bool is_zero() const {
            return value == 0;
        }

        bool is_one() const {
            return value == 1;
        }

        Modular& operator=(Modular copy) {
            value = copy.value;
            return (*this);
        }

        Modular(const Modular &copy) : value(copy.value) {
        }
    };
}