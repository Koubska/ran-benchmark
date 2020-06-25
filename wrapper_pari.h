#pragma once

/**
 * PARI (from PARI/GP).
 * On Ubuntu, install libpari-dev
 */

#include "wrapper_abstract.h"

#include <pari/pari.h>

#include <iostream>
#include <vector>

namespace pari {

std::ostream& operator<<(std::ostream& os, const GEN& g) {
    return os << GENtostr(g);
}

class Poly {
    friend std::ostream& operator<<(std::ostream& os, const Poly& p);
private:
    GEN mPoly;
public:
    Poly(const GEN& g): mPoly(g) {}
    Poly(int i, long x, unsigned n): mPoly(gmul(stoi(i), gpowgs(pol_x(x), n))) {}

    auto poly() const {
        return mPoly;
    }
};

std::ostream& operator<<(std::ostream& os, const Poly& p) {
    return os << p.poly();
}

Poly operator+(const Poly& lhs, const Poly& rhs) {
    return gadd(lhs.poly(), rhs.poly());
}
Poly operator-(const Poly& lhs, const Poly& rhs) {
    return gsub(lhs.poly(), rhs.poly());
}
Poly operator*(const Poly& lhs, const Poly& rhs) {
    return gmul(lhs.poly(), rhs.poly());
}

class PariWrapper : public Wrapper {
    std::vector<long> vars;
public:
    using Poly = pari::Poly;
    
    PariWrapper() {
        pari_init(1024*1024*128, 500000);
    }

    auto fresh_variable(const char* name) {
        vars.emplace_back(fetch_user_var(name));
        return vars.back();
    }

    void set_variable(long var, PolyInit poly, int low, int high) {
        GEN v = pol_x(var);
        GEN defp = stoi(0);
        std::size_t exp = 0;
        for (auto coeff: poly) {
            defp = gadd(defp, gmul(stoi(coeff), gpowgs(v, exp)));
            ++exp;
        }
        //std::cout << defp << std::endl;

        auto res = realroots(defp, nullptr, 4);
        //std::cout << res << std::endl;
    }

    std::vector<int> lift(const Poly& p, long var) {
        return {1,2,3};
    }
};
}