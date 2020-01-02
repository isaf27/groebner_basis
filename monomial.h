#include <cassert>
#include <vector>
#include <iostream>

namespace polynomial {

    template<uint32_t size>
    class Monomial {
    private:
        std::vector<uint32_t> degree;

    public:
        Monomial() : degree(size, 0) {
        }

        Monomial(std::vector<uint32_t> degree) : degree(degree) {
        }

        Monomial operator*(const Monomial& other) const {
            std::vector<uint32_t> result(size, 0);
            for (uint32_t i = 0; i < size; ++i) {
                result[i] = degree[i] + other.degree[i];
            }
            return Monomial<size>(result);
        }

        Monomial operator/(const Monomial& other) const {
            std::vector<uint32_t> result(size, 0);
            for (uint32_t i = 0; i < size; ++i) {
                assert(((void)"divider should be a subset of dividend", degree[i] >= other.degree[i]));
                result[i] = degree[i] - other.degree[i];
            }
            return Monomial<size>(result);
        }

        bool operator==(const Monomial& other) const {
            return degree == other.degree;
        }

        bool operator<(const Monomial& other) const {
            return degree < other.degree;
        }

        bool is_subset(const Monomial& other) const {
            for (uint32_t i = 0; i < size; ++i) {
                if (degree[i] < other.degree[i]) {
                    return false;
                }
            }
            return true;
        }

        bool is_empty() const {
            for (uint32_t i = 0; i < size; ++i) {
                if (degree[i] != 0) {
                    return false;
                }
            }
            return true;
        }

        uint32_t get_degree(uint32_t num) const {
            assert(((void)"degree should be between 0 and size - 1", (size > num && num >= 0)));
            return degree[num];
        }

        friend std::ostream& operator<<(std::ostream &out, const Monomial &element) {
            uint32_t non_zero_count = 0;
            for (uint32_t i = 0; i < size; ++i) {
                non_zero_count += element.degree[i] > 0;
            }
            if (non_zero_count == 0) {
                out << "1";
            } else {
                for (uint32_t i = 0; i < size; ++i) {
                    if (element.degree[i] > 0) {
                        out << "x_" << i + 1;
                        if (element.degree[i] > 1) {
                            out << "^" << element.degree[i];
                        }
                        non_zero_count--;
                        if (non_zero_count > 0) {
                            out << "*";
                        }
                    }
                }
            }
            return out;
        }

        Monomial& operator=(Monomial copy) {
            degree = copy.degree;
            return (*this);
        }

        Monomial(const Monomial &copy) : degree(copy.degree) {
        }
    };
}