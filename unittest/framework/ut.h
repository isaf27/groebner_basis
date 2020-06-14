#ifndef GROEBNER_BASIS_UT_H
#define GROEBNER_BASIS_UT_H

#include <sstream>
#include <exception>
#include <iostream>
#include <string>

template<class T, class U>
void assert_equal(const T& t, const U& u, const std::string& hint = "") {
    if (t != u) {
        std::ostringstream os;
        os << "Assertion failed: " << t << " != " << u << " hint: " << hint;
        throw std::runtime_error(os.str());
    }
}

void make_assert(bool b, const std::string& hint = "") {
    if (!b) {
        std::ostringstream os;
        os << "Assertion failed, the statement is false, hint: " << hint;
        throw std::runtime_error(os.str());
    }
}

class TestRunner {
public:
    template <class TestFunc>
    void run_test(TestFunc func, const std::string& test_name) {
        ++all_count_;
        try {
            func();
            std::cerr << test_name << " OK" << std::endl;
        } catch (std::runtime_error& e) {
            ++fail_count_;
            std::cerr << test_name << " FAIL: " << e.what() << std::endl;
        }
    }

    ~TestRunner() {
        if (fail_count_ > 0) {
            std::cerr << fail_count_ << " of " << all_count_ << " unit tests failed. Terminate." << std::endl;
            exit(1);
        } else {
            std::cerr << all_count_ << " tests passed! Good job." << std::endl;
            exit(0);
        }
    }

private:
    int fail_count_ = 0;
    int all_count_ = 0;
};

#endif
