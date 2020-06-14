#include "framework/ut.h"

#include "../fields/modular.h"

using namespace math;

void test_modulo_2() {
    Modular<2> zero = 0;
    Modular<2> one = 1;
    assert_equal(one + one, zero, "2 * 1 == 0");
    assert_equal(zero - one, one, "0 - 1 == 1");
    assert_equal(zero * one, zero, "0 * 1 == 0");
    assert_equal(zero * one, one * zero, "0 * 1 == 1 * 0");
    assert_equal(zero / one, zero, "0 / 1 == 0");
    assert_equal(one / one, one, "1 / 1 == 1");
    assert_equal(one * one * one * one, one, "1 * 1 * 1 * 1 == 1");
    assert_equal(one * one * one * one, one, "1 * 1 * 1 * 1 == 1");
    make_assert(one.is_one(), "1 is one");
    make_assert(!one.is_zero(), "1 is not zero");
    one += one;
    make_assert(one.is_zero(), "0 is zero");
    make_assert(!one.is_one(), "0 is not one");
    assert_equal(one, zero, "1 += 1 is equal to 0");
    one -= 1;
    assert_equal(one, zero + 1, "1 is equal to 1");
}

constexpr uint32_t MOD = 1e9 + 7;

void test_big_modulo() {
    Modular<MOD> zero = 0;
    Modular<MOD> one = 1;
    Modular<MOD> a = 239;
    Modular<MOD> b = MOD - 1;
    Modular<MOD> c = a * a / 2;
    assert_equal(one + one, 2, "1 + 1 == 2");
    assert_equal(c * 2 / a / a, one, "division check");
    make_assert((b + one).is_zero(), "-1 + 1 == 0");
    make_assert((b * b).is_one(), "-1 * -1 == 1");
}

int main() {
    TestRunner runner;
    runner.run_test(test_modulo_2, "Modulo 2 test");
    runner.run_test(test_big_modulo, "Big modulo (10^9+7) test");
    return 0;
}