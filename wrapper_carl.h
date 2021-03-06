#pragma once

/*
 * Wrapper for carl. Download and build carl somewhere and adapt the path
 * here.
 */

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "../carl/src/carl-model/evaluation/ModelEvaluation_Polynomial.h"
#include "../carl/src/carl/core/MultivariatePolynomial.h"
#include "../carl/src/carl/core/UnivariatePolynomial.h"
#include "../carl/src/carl/core/Variables.h"
#include "../carl/src/carl/core/polynomialfunctions/Evaluation.h"
#include "../carl/src/carl/core/polynomialfunctions/Resultant.h"
#include "../carl/src/carl/core/polynomialfunctions/to_univariate_polynomial.h"
#include "../carl/src/carl/ran/interval/ran_interval_real_roots.h"
#include "../carl/src/carl/ran/ran.h"
#include "../carl/src/carl/ran/real_roots_common.h"

namespace carl {

using MPoly = carl::MultivariatePolynomial<mpq_class>;
using UMPoly = carl::UnivariatePolynomial<MPoly>;
using UPoly = carl::UnivariatePolynomial<mpq_class>;
using RAN = carl::RealAlgebraicNumber<mpq_class>;
using Var = carl::Variable;
using Assignment = std::map<Var, RAN>;

class UniPoly {
  friend std::ostream &operator<<(std::ostream &os, const UniPoly &p);

private:
  UPoly uPoly;

public:
  // UniPoly() {} UnivariatePolynomial does not have the empty Constructor
  UniPoly(UPoly &&poly) : uPoly(std::move(poly)) {}
  UniPoly(int coeff, Var var, unsigned int pow) : uPoly(var, coeff, pow) {}

  const std::string type() const { return "Carl Uni"; }

  const UPoly poly() const { return uPoly; }
  UPoly poly() { return uPoly; }
};

class UniMultiPoly {
  friend std::ostream &operator<<(std::ostream &os, const UniMultiPoly &p);

private:
  std::shared_ptr<UMPoly> uPoly;

public:
  UniMultiPoly() : uPoly() {}
  UniMultiPoly(UMPoly &&poly) { uPoly = std::make_shared<UMPoly>(poly); }

  const std::string type() const { return "Carl UniMulti"; }

  const UMPoly poly() const { return *uPoly; }
  UMPoly poly() { return *uPoly; }
};

class MultiPoly {
  friend std::ostream &operator<<(std::ostream &os, const MultiPoly &p);

private:
  MPoly mPoly;

public:
  MultiPoly() : mPoly() {}
  MultiPoly(UMPoly poly) : mPoly(poly) {}
  MultiPoly(int constant) : mPoly(constant) {} // Create Constant Poly
  MultiPoly(Var var) : mPoly(var) {}
  MultiPoly(MPoly &&poly) : mPoly(std::move(poly)) {}
  MultiPoly(int i, Var x, unsigned int n)
      : mPoly(MPoly(i) * carl::pow(MPoly(x), n)) {}

  const std::string type() const { return "Carl Multi"; }

  const MPoly poly() const { return mPoly; }
  MPoly poly() { return mPoly; }

  std::vector<Var> getVariables() { return carl::variables(mPoly).as_vector(); }
};

std::ostream &operator<<(std::ostream &os, const UniMultiPoly &p) {
  return os << p.poly();
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

UMPoly to_univariate_polynomial(const MultiPoly &poly, const Var &mainVar) {
  return carl::to_univariate_polynomial(poly.poly(), mainVar);
}

MultiPoly resultant(const MultiPoly &lhs, const MultiPoly &rhs,
                    const Var &mainVar) {
  return MultiPoly(carl::resultant(to_univariate_polynomial(lhs, mainVar),
                                   to_univariate_polynomial(rhs, mainVar)));
}

UniPoly resultant(const UniPoly &lhs, const UniPoly &rhs) {
  return carl::resultant(lhs, rhs);
}

// Assert mainVar(lhs) == mainVar(rhs)
UMPoly resultant(const UniMultiPoly &lhs, const UniMultiPoly &rhs) {
  return carl::resultant(lhs.poly(), rhs.poly());
}

std::vector<RAN> isolate_roots(const MultiPoly &poly,
                               const Assignment &assignment,
                               const Var &mainVar) {
  // poly needs to be univariate when evaluated over assignemnt
  // mainVar must not be in assignment
  // TODO : is that the right function? (carl/ran/interval/RealRootIsolation.h)
  return carl::real_roots(to_univariate_polynomial(poly, mainVar), assignment)
      .roots();
}

std::vector<RAN> isolate_roots(const UPoly &poly) {
  // TODO : is that the right function? (carl/ran/interval/RealRootIsolation.h)
  return carl::real_roots(poly).roots();
}

class CarlWrapper {
  std::map<std::string, Var> variables;

public:
  using MultiPoly = carl::MultiPoly;
  using UniPoly = carl::UniPoly;
  using UniMultiPoly = carl::UniMultiPoly;
  using Assignment = carl::Assignment;
  using RAN = carl::RAN;

  Var fresh_variable(std::string name) {
    if (!variables.count(name)) {
      variables[name] = carl::freshRealVariable(std::string(name));
    }
    return variables[name];
  }

  std::map<std::string, Var> &get_variables() { return variables; }

  std::vector<std::string> get_variable_names() {
    std::vector<std::string> names;
    for (const auto &items : variables) {
      names.push_back(items.first);
    }
    return names;
  }

  // Assumes var_name.size() == values.size()
  void build_assignment(std::shared_ptr<Assignment> ass,
                        std::vector<std::string> var_name,
                        std::vector<std::string> values) {

    for (size_t i = 0; i < var_name.size(); ++i) {
      ass->insert(
          std::make_pair(fresh_variable(var_name[i]), build_RAN(values[i])));
    }
  }

  template <typename T> RAN build_RAN(T number) {
    return RAN((mpq_class)number);
  }
};

} // namespace carl
