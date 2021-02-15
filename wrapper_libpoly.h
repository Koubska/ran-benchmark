#pragma once

/*
 * Wrapper for libpoly. Download and build libpoly somewhere and adapt the path
 * here.
 */

#include <iostream>
#include <map>

#include "../libpoly/include/polyxx.h"

namespace libpoly {

using UPoly = poly::UPolynomial;
using MPoly = poly::Polynomial;
using Var = poly::Variable;

class UniPoly {
  friend std::ostream &operator<<(std::ostream &os, const UniPoly &p);

private:
  UPoly uPoly;

public:
  UniPoly() {} // Zero Poly
  UniPoly(int constant)
      : uPoly(poly::Integer(constant)) {}           // Create Constant Poly
  UniPoly(UPoly &&poly) : uPoly(std::move(poly)) {} // Copy Constructor
  UniPoly(int coeff, Var x, unsigned int pow)
      : uPoly(pow, coeff) {} // coeff * x^pow TODO var is not used

  const UPoly poly() const { return uPoly; }
  UPoly poly() { return uPoly; }
};

class MultiPoly {
  friend std::ostream &operator<<(std::ostream &os, const MultiPoly &p);

private:
  MPoly mPoly;

public:
  MultiPoly() {}
  MultiPoly(int constant)
      : mPoly(poly::Integer(constant)) {}             // Create Constant Poly
  MultiPoly(MPoly &&poly) : mPoly(std::move(poly)) {} // Copy Constructor
  MultiPoly(int coeff, Var var, unsigned int pow)
      : mPoly(poly::Integer(coeff), var, pow) {}

  const MPoly poly() const { return mPoly; }
  MPoly poly() { return mPoly; }
};

std::ostream &operator<<(std::ostream &os, const MultiPoly &p) {
  return os << p.poly();
}

MultiPoly operator+(const MultiPoly &lhs, const MultiPoly &rhs) {
  return MultiPoly(lhs.poly() + rhs.poly());
}

MultiPoly operator-(const MultiPoly &lhs, const MultiPoly &rhs) {
  return MultiPoly(lhs.poly() - rhs.poly());
}

MultiPoly operator*(const MultiPoly &lhs, const MultiPoly &rhs) {
  return MultiPoly(lhs.poly() * rhs.poly());
}

std::ostream &operator<<(std::ostream &os, const UniPoly &p) {
  return os << p.poly();
}

UniPoly operator+(const UniPoly &lhs, const UniPoly &rhs) {
  return UniPoly(lhs.poly() + rhs.poly());
}

UniPoly operator-(const UniPoly &lhs, const UniPoly &rhs) {
  return UniPoly(lhs.poly() - rhs.poly());
}

UniPoly operator*(const UniPoly &lhs, const UniPoly &rhs) {
  return UniPoly(lhs.poly() * rhs.poly());
}

class LibPolyWrapper {
public:
  using MultiPoly = libpoly::MultiPoly;
  using UniPoly = libpoly::UniPoly;

  Var fresh_variable(const char *name) { return Var(name); }
};

} // namespace libpoly