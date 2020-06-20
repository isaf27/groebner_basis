#include "../fields/rational.h"
#include "../fields/modular.h"
#include "../library/ideal.h"

#include <iostream>
#include <ctime>

using namespace math;
using namespace polynomial;
using namespace std;

#define TIME (clock() * 1.0 / CLOCKS_PER_SEC)

Polynomial<Rational> get_sigma(int n, int k) {
    Polynomial<Rational> result;
    for (int mask = 0; mask < (1 << n); ++mask) {
        vector<uint32_t> deg(n, 0);
        int sz = 0;
        for (int i = 0; i < n; ++i) {
            if ((mask >> i) & 1) {
                deg[i]++;
                sz++;
            }
        }
        if (sz != k) {
            continue;
        }
        result.add(Monomial(std::move(deg)), 1);
    }
    return result;
}

void root_n(int n) {
    double start_time = TIME;
    cout << "root_n test for n = " << n << endl;
    Ideal<Rational> ideal;
    for (int k = 1; k < n; ++k) {
        ideal.add(get_sigma(n, k));
    }
    Polynomial<Rational> s_n = get_sigma(n, n);
    int coef = (n % 2 == 1 ? -1 : 1);
    s_n.add({}, coef);
    ideal.add(s_n);
    ideal.make_minimal_groebner_basis();
    cout << "working time: " << TIME - start_time << endl;
}

void test_root_n(int n) {
    for (int i = 1; i <= n; ++i) {
        root_n(i);
    }
}

constexpr int MOD = 239;

Polynomial<Modular<MOD>> get_cyclic(int n, int k) {
    Polynomial<Modular<MOD>> result;
    for (int i = 0; i < n; ++i) {
        vector<uint32_t> deg(n, 0);
        for (int j = 0; j < k; ++j) {
            deg[(i + j) % n] = 1;
        }
        result.add(Monomial(std::move(deg)), 1);
    }
    return result;
}

void cyclic_n(int n) {
    double start_time = TIME;
    cout << "cyclic_n test for n = " << n << endl;
    Ideal<Modular<MOD>> ideal;
    for (int k = 1; k < n; ++k) {
        ideal.add(get_cyclic(n, k));
    }
    Polynomial<Modular<MOD>> s_n = get_cyclic(n, n);
    s_n.subtract({}, 1);
    ideal.add(s_n);
    ideal.make_groebner_basis();
    cout << "working time: " << TIME - start_time << endl;
}

void test_cyclic_n(int n) {
    for (int i = 1; i <= n; ++i) {
        cyclic_n(i);
    }
}

int main() {
    test_root_n(10);
    test_cyclic_n(6);
}
