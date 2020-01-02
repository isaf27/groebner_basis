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

        Polynomial operator/(const Field& coefficient) const {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            std::map<Monomial<size>, Field, Compare> result = monomials;
            for (const auto& monomial : monomials) {
                result[monomial.first] /= coefficient;
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator/=(const Field& coefficient) {
            assert(((void)"divizion by zero", !coefficient.is_zero()));
            for (const auto& monomial : monomials) {
                monomials[monomial.first] /= coefficient;
            }
            return *this;
        }

        Polynomial operator*(const Monomial<size>& monomial) const {
            std::map<Monomial<size>, Field, Compare> result;
            for (const auto& element : monomials) {
                result[element.first * monomial] = element.second;
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator*=(const Monomial<size>& monomial) {
            *this = *this * monomial;
            return *this;
        }

        Polynomial operator*(const Polynomial& other) const {
            std::map<Monomial<size>, Field, Compare> result;
            for (const auto& first_monomial : monomials) {
                for (const auto& second_monomial : other.monomials) {
                    const auto multiply = first_monomial.first * second_monomial.first;
                    result[multiply] += first_monomial.second * second_monomial.second;
                    if (result[multiply] == 0) {
                        result.erase(multiply);
                    }
                }
            }
            return Polynomial<size, Field, Compare>(std::move(result));
        }

        Polynomial operator*=(const Polynomial& other) {
            *this = *this * other;
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

        bool is_zero() const {
            return monomials.empty();
        }

        Monomial<size> get_major_monomial() const {
            if (is_zero()) {
                return Monomial<size>();
            } else {
                return monomials.rbegin()->first;
            }
        }

        Field get_major_coefficient() const {
            if (is_zero()) {
                return 0;
            } else {
                return monomials.rbegin()->second;
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