#include "monomial.h"
#include <functional>
#include <map>
#include <initializer_list>
#include <iostream>
#include <utility>

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

        Polynomial(std::map<Monomial<size>, Field, Compare>&& monomials) : monomials(monomials) {
        }

        Polynomial(std::initializer_list<std::pair<Monomial<size>, Field>> elements) {
            for (const auto& element : elements) {
                if (!element.second.is_zero()) {
                    monomials[element.first] += element.second;
                    if (monomials[element.first] == 0) {
                        monomials.erase(element.first);
                    }
                }
            }
        }

        bool operator==(const Polynomial& other) const {
            return monomials == other.monomials;
        }

        Polynomial operator+(const Polynomial& other) const {
            std::map<Monomial<size>, Field, Compare> result = monomials;
            for (const auto& monomial : other.monomials) {
                result[monomial.first] += monomial.second;
                if (result[monomial.first] == 0) {
                    result.erase(monomial.first);
                }
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator+=(const Polynomial& other) {
            for (const auto& monomial : other.monomials) {
                monomials[monomial.first] += monomial.second;
                if (monomials[monomial.first] == 0) {
                    monomials.erase(monomial.first);
                }
            }
            return *this;
        }

        Polynomial operator-(const Polynomial& other) const {
            std::map<Monomial<size>, Field, Compare> result = monomials;
            for (const auto& monomial : other.monomials) {
                result[monomial.first] -= monomial.second;
                if (result[monomial.first] == 0) {
                    result.erase(monomial.first);
                }
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator-=(const Polynomial& other) {
            for (const auto& monomial : other.monomials) {
                monomials[monomial.first] -= monomial.second;
                if (monomials[monomial.first] == 0) {
                    monomials.erase(monomial.first);
                }
            }
            return *this;
        }

        Polynomial operator*(const Field& coefficient) const {
            if (coefficient.is_zero()) {
                return Polynomial<size, Field, Compare>();
            }
            std::map<Monomial<size>, Field, Compare> result = monomials;
            for (const auto& monomial : monomials) {
                result[monomial.first] *= coefficient;
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator*=(const Field& coefficient) {
            if (coefficient.is_zero()) {
                monomials.clear();
            } else {
                for (const auto& monomial : monomials) {
                    monomials[monomial.first] *= coefficient;
                }
            }
            return *this;
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
                    out << " + ";
                }
            }
            return out;
        }

        Monomial<size> get_major_monomial() const {
            if (monomials.empty()) {
                return Monomial<size>();
            } else {
                return monomials.rbegin()->first;
            }
        }

        Polynomial& operator=(Polynomial copy) {
            monomials = copy.monomials;
            return (*this);
        }

        Polynomial(const Polynomial &copy) : monomials(copy.monomials) {
        }
    };
}