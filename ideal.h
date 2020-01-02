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

        Ideal(const std::vector<Polynomial<size, Field, Compare>>& polynomials) : polynomials(polynomials) {
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