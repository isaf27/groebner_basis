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

        void add(const Polynomial<size, Field, Compare>& polynomial) {
            if (!polynomial.is_zero()) {
                polynomials.push_back(polynomial);
                polynomials.back() /= polynomials.back().get_major_coefficient();
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

        void make_groebner_basis() {
            for (uint32_t i = 0; i < polynomials.size(); ++i) {
                for (uint32_t j = 0; j < i; ++j) {
                    const auto& intersection = get_intersection(
                        polynomials[i].get_major_monomial(),
                        polynomials[j].get_major_monomial()
                    );
                    if (intersection.is_empty()) {
                        continue;
                    }
                    auto s_polynomial = polynomials[i] * (polynomials[j].get_major_monomial() / intersection);
                    s_polynomial -= polynomials[j] * (polynomials[i].get_major_monomial() / intersection);
                    this->reduce(s_polynomial);
                    this->add(s_polynomial);
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