#pragma once

/*
 * Wrapper for carl.
 */

#include "../carl/src/carl/core/MultivariatePolynomial.h"
#include "../carl/src/carl/core/UnivariatePolynomial.h"
#include "../carl/src/carl/formula/model/ran/real_roots.h"
#include "../carl/src/carl/core/polynomialfunctions/to_univariate_polynomial.h"
#include "../carl/src/carl/formula/model/ran/RealAlgebraicNumber.h"
#include "../carl/src/carl-model/Model.h"

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

#include "wrapper_abstract.h"

namespace carl {

    using MPoly = typename carl::MultivariatePolynomial<mpq_class>;
    using UMPoly = typename carl::UnivariatePolynomial<MPoly>;
    using UPoly = typename carl::UnivariatePolynomial<mpq_class>;
    using RAN = carl::RealAlgebraicNumber<mpq_class>;

    class Poly {
        friend std::ostream& operator<<(std::ostream& os, const Poly& p);
    private:
        MPoly mPoly;
    public:
        Poly(MPoly&& poly): mPoly(std::move(poly)) {}
        Poly(int i, carl::Variable x, unsigned n): mPoly(MPoly(i) * carl::pow(MPoly(x), n)) {
        }

        auto poly() const {
            return mPoly;
        }
    };
    
    std::ostream& operator<<(std::ostream& os, const Poly& p) {
        return os << p.poly();
    }

    Poly operator+(const Poly& lhs, const Poly& rhs) {
        return Poly(lhs.poly() + rhs.poly());
    }
    Poly operator-(const Poly& lhs, const Poly& rhs) {
        return Poly(lhs.poly() - rhs.poly());
    }
    Poly operator*(const Poly& lhs, const Poly& rhs) {
        return Poly(lhs.poly() * rhs.poly());
    }

    Poly resultant(const Poly& p, const Poly& q) {
        return Poly(carl::resultant(p.poly(), q.poly()));
    }

    class CarlWrapper: public Wrapper {
        std::map<carl::Variable, RAN> mModel;
    public:        
        using Poly = carl::Poly;

        carl::Variable fresh_variable(const char* name) {
            return carl::freshRealVariable(std::string(name));
        }

        void set_variable(carl::Variable var, PolyInit poly, int low, int high) {
            std::vector<mpq_class> coeffs;
            for (int c: poly) coeffs.emplace_back(c);
            UPoly p(var, std::move(coeffs));
            RAN ran(p, Interval<mpq_class>(low, BoundType::STRICT, high, BoundType::STRICT));
            mModel[var] =  ran;
        }

        std::vector<RAN> isolate_real_roots(const Poly& p, carl::Variable v) {
            UMPoly poly = carl::to_univariate_polynomial(p.poly(), v);
            return carl::realRoots(poly, mModel);
        }

        std::vector<RAN> lift(const Poly& p, carl::Variable v) {
            auto roots = isolate_real_roots(p, v);
            if (roots.size() == 0) {
                return {RAN(0)};
            }
            std::vector<RAN> samples;
            samples.emplace_back(sample_below(roots[0]));
            samples.emplace_back(roots[0]);
            for (std::size_t i = 1; i < roots.size(); ++i) {
                samples.emplace_back(sample_between(samples.back(), roots[i]));
                samples.emplace_back(roots[i]);
            }
            samples.emplace_back(sample_above(samples.back()));
            return samples;
        }
    };


}
