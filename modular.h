#include <cassert>
#include <iostream>

namespace math {

    template <uint32_t modulo>
    class modular {
    private:
        uint32_t value;

        modular power(uint32_t degree) const {
            if (degree == 0) {
                return modular<modulo>(1);
            }
            const auto subpower = this->power(degree / 2);
            if (degree % 2 == 0) {
                return subpower * subpower;
            } else {
                return subpower * subpower * (*this);
            }
        }

        modular inverse() const {
            assert(((void)"division by zero", value != 0));
            return this->power(modulo - 2);
        }

    public:
        modular(uint32_t value = 0) : value(value) {
            assert(((void)"value should be less than modulo", value < modulo));
        }

        bool operator==(const modular& other) const {
            return value == other.value;
        }

        modular operator+(const modular& other) const {
            if (value + other.value >= modulo) {
                return modular<modulo>(value + other.value - modulo);
            } else {
                return modular<modulo>(value + other.value);
            }
        }

        modular operator-(const modular& other) const {
            if (value >= other.value) {
                return modular<modulo>(value - other.value);
            } else {
                return modular<modulo>(value + modulo - other.value);
            }
        }

        modular operator*(const modular& other) const {
            return modular<modulo>((uint64_t)value * (uint64_t)other.value % (uint64_t)modulo);
        }

        modular operator/(const modular& other) const {
            return (*this) * other.inverse();
        }

        friend std::ostream& operator<<(std::ostream &out, const modular &element) {
            out << element.value << " (modulo " << modulo << ")";
            return out;
        }

        modular& operator=(modular copy) {
            value = copy.value;
        }

        modular(const modular &copy) : value(copy.value) {
        }
    };
}