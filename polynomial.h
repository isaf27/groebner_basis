#include "monomial.h"
#include <functional>
#include <map>
#include <iostream>

namespace polynomial {

    template<uint32_t size, class Field, class Compare = std::less<Monomial<size>>>
    class Polynomial {
    private:
        std::map<Monomial<size>, Field, Compare> monomials;

    public:
        Polynomial() : monomials({}) {
        }

        Polynomial(const Monomial<size>& monomial, const Field& coefficient) {
            if (!coefficient.is_zero()) {
                monomials[monomial] = coefficient;
            }
        }

        Polynomial(const std::vector<std::pair<Monomial<size>, Field>>& elements) {
            for (const auto& element : elements) {
                if (!element.second.is_zero()) {
                    monomials[element.first] += element.second;
                    if (monomials[element.first] == 0) {
                        monomials.erase(element.first);
                    }
                }
            }
        }

        friend std::ostream& operator<<(std::ostream &out, const Polynomial &element) {
            uint32_t monomials_count = element.monomials.size();
            for (const auto& monomial : element.monomials) {
                monomials_count--;
                if (monomial.first.is_empty() && monomial.second.is_one()) {
                    out << Field(1);
                } else {
                    if (!monomial.second.is_one()) {
                        out << monomial.second;
                        if (!monomial.first.is_empty()) {
                            out << "*";
                        }
                    }
                    if (!monomial.first.is_empty()) {
                        out << monomial.first;
                    }
                }
                if (monomials_count > 0) {
                    out << "+";
                }
            }
            return out;
        }

        Polynomial& operator=(Polynomial copy) {
            monomials = copy.monomials;
            return (*this);
        }

        Polynomial(const Polynomial &copy) : monomials(copy.monomials) {
        }
    };
}