#include "polynomial.h"
#include <vector>

namespace polynomial {

    template<uint32_t size, class Field, class Compare = std::less<Monomial<size>>>
    class Ideal {
    private:
        std::vector<Polynomial<size, Field, Compare>> polynomials;

    public:
        Ideal() : polynomials({}) {
        }

        Ideal(const std::vector<Polynomial<size, Field, Compare>>& elements) {
            for (const auto& polynomial : elements) {
                if (!polynomial.is_zero()) {
                    polynomials.push_back(polynomial);
                    polynomials.back() /= polynomials.back().get_major_coefficient();
                }
            }
        }

        void reduce(Polynomial<size, Field, Compare>& polynomial) {
            while (!polynomial.is_zero()) {
                const auto& major_monomial = polynomial.get_major_monomial();
                bool was_reduced = false;
                for (const auto& reducer : polynomials) {
                    if (major_monomial.is_subset(reducer.get_major_monomial())) {
                        polynomial -= reducer * (major_monomial / reducer.get_major_monomial()) * polynomial.get_major_coefficient();
                        was_reduced = true;
                        break;
                    }
                }
                if (!was_reduced) {
                    break;
                }
            }
        }

        friend std::ostream& operator<<(std::ostream &out, const Ideal &ideal) {
            out << "(";
            uint32_t polynomials_count = ideal.polynomials.size();
            for (const auto& polynomial : ideal.polynomials) {
                out << polynomial;
                polynomials_count--;
                if (polynomials_count > 0) {
                    out << ", ";
                }
            }
            out << ")";
            return out;
        }

        Ideal& operator=(Ideal copy) {
            polynomials = copy.polynomials;
            return (*this);
        }

        Ideal(const Ideal &copy) : polynomials(copy.polynomials) {
        }
    };
}