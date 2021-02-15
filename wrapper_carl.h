#pragma once

/*
 * Wrapper for carl. Download and build carl somewhere and adapt the path
 * here.
 */

#include <iostream>
#include <map>

#include "../carl/src/carl-model/Model.h"
#include "../carl/src/carl/core/MultivariatePolynomial.h"
#include "../carl/src/carl/core/UnivariatePolynomial.h"
#include "../carl/src/carl/core/polynomialfunctions/to_univariate_polynomial.h"
#include "../carl/src/carl/ran/interval/ran_interval_real_roots.h"

namespace carl {

using MPoly = carl::MultivariatePolynomial<mpq_class>;
using UMPoly = carl::UnivariatePolynomial<MPoly>;
using UPoly = carl::UnivariatePolynomial<mpq_class>;
using RAN = carl::RealAlgebraicNumber<mpq_class>;
using Var = carl::Variable;

class UniPoly {
  friend std::ostream& operator<<(std::ostream& os, const UniPoly& p);

 private:
  UPoly uPoly;

 public:
  // Poly() {} UnivariatePolynomial does not have the empty Constructor
  UniPoly(UPoly&& poly) : uPoly(std::move(poly)) {}
  UniPoly(int coeff, Var var, unsigned int pow) : uPoly(var, coeff, pow) {}

  const UPoly poly() const { return uPoly; }
  UPoly poly() { return uPoly; }
};

class MultiPoly {
  friend std::ostream& operator<<(std::ostream& os, const MultiPoly& p);

 private:
  MPoly mPoly;

 public:
  MultiPoly() : mPoly() {}
  MultiPoly(int constant) : mPoly(constant) {}  // Create Constant Poly
  MultiPoly(Var var) : mPoly(var) {}
  MultiPoly(MPoly&& poly) : mPoly(std::move(poly)) {}
  MultiPoly(int i, Var x, unsigned int n)
      : mPoly(MPoly(i) * carl::pow(MPoly(x), n)) {}

  const std::string type() const {
    return "Carl" ;
  }

  const MPoly poly() const { return mPoly; }
  MPoly poly() { return mPoly; }
};

std::ostream& operator<<(std::ostream& os, const UniPoly& p) {
  return os << p.poly();
}

UniPoly operator+(const UniPoly& lhs, const UniPoly& rhs) {
  return UniPoly(lhs.poly() + rhs.poly());
}

UniPoly operator-(const UniPoly& lhs, const UniPoly& rhs) {
  return UniPoly(lhs.poly() - rhs.poly());
}

UniPoly operator*(const UniPoly& lhs, const UniPoly& rhs) {
  return UniPoly(lhs.poly() * rhs.poly());
}

std::ostream& operator<<(std::ostream& os, const MultiPoly& p) {
  return os << p.poly();
}

MultiPoly operator+(const MultiPoly& lhs, const MultiPoly& rhs) {
  return MultiPoly(lhs.poly() + rhs.poly());
}

MultiPoly operator-(const MultiPoly& lhs, const MultiPoly& rhs) {
  return MultiPoly(lhs.poly() - rhs.poly());
}

MultiPoly operator*(const MultiPoly& lhs, const MultiPoly& rhs) {
  return MultiPoly(lhs.poly() * rhs.poly());
}

class CarlWrapper {
  std::map<std::string, Var> variables;

 public:
  using MultiPoly = carl::MultiPoly;
  using UniPoly = carl::UniPoly;

  Var fresh_variable(const std::string name) {
    return carl::freshRealVariable(std::string(name));
  }
};

}  // namespace carl
