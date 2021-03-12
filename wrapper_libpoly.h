#pragma once

/*
 * Wrapper for libpoly. Download and build libpoly somewhere and adapt the path
 * here.
 */

#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "../libpoly/include/algebraic_number.h"
#include "../libpoly/include/poly.h"
#include "../libpoly/include/polynomial.h"
#include "../libpoly/include/polynomial_context.h"
#include "../libpoly/include/polyxx.h"
#include "../libpoly/include/variable_order.h"

namespace libpoly {

using UPoly = poly::UPolynomial;
using MPoly = poly::Polynomial;
using Var = poly::Variable;
using RAN = poly::AlgebraicNumber;
using Assignment = poly::Assignment;

class UniPoly {
  friend std::ostream &operator<<(std::ostream &os, const UniPoly &p);

private:
  UPoly uPoly;

public:
  UniPoly(int constant)
      : uPoly(poly::Integer(constant)) {}           // Create Constant Poly
  UniPoly(UPoly &&poly) : uPoly(std::move(poly)) {} // Copy Constructor
  UniPoly(int coeff, Var x, unsigned int pow)
      : uPoly(pow, coeff) {} // coeff * x^pow TODO var is not used

  const std::string type() const { return "LibPoly Uni"; }

  const UPoly poly() const { return uPoly; }
  UPoly poly() { return uPoly; }
};

class MultiPoly {
  friend std::ostream &operator<<(std::ostream &os, const MultiPoly &p);

private:
  MPoly mPoly;

public:
  MultiPoly() : mPoly() {} // Construct Zero Polynomial
  MultiPoly(int constant)
      : mPoly(poly::Integer(constant)) {}             // Create Constant Poly
  MultiPoly(MPoly &&poly) : mPoly(std::move(poly)) {} // Copy Constructor
  MultiPoly(int coeff, Var var, unsigned int pow)
      : mPoly(poly::Integer(coeff), var, pow) {}

  const std::string type() const { return "LibPoly Multi"; }

  const MPoly poly() const { return mPoly; }
  MPoly poly() { return mPoly; }
};

class Assign {
  friend std::ostream &operator<<(std::ostream &os, const Assign &p);

private:
  Assignment ass;

public:
  Assign() : ass() {} // Construct empty Assignment

  const std::string type() const { return "LibPoly Assignment"; }

  const Assignment assignment() const { return ass; }
  Assignment assignment() { return ass; }

  void set(const poly::Variable &var, const poly::Value &val) {
    ass.set(var, val);
  }

  void unset(const poly::Variable &var) { ass.unset(var); }
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

auto resultant(const MultiPoly &lhs, const MultiPoly &rhs, const Var &mainVar) {
  // lhs.poly().get_internal()->ctx->var_order->top = mainVar.get_internal();
  // rhs.poly().get_internal()->ctx->var_order->top = mainVar.get_internal();
  //auto test1 = lp_polynomial_get_context(lhs.poly().get_internal());
  //auto test2 = lp_polynomial_get_context(rhs.poly().get_internal());
  //lp_variable_order_push(
  //    lp_polynomial_get_context(lhs.poly().get_internal())->var_order,
  //    mainVar.get_internal()); // Does not work
  //std::cout << lp_polynomial_context_equal(test1, test2) << std::endl;
  std::cout << poly::main_variable(lhs.poly()) << std::endl;
  std::cout << poly::main_variable(rhs.poly()) << std::endl;
  std::cout << mainVar << std::endl;

  // TODO still unclear how to set the main variable
  return poly::resultant(lhs.poly(), rhs.poly());
}

class LibPolyWrapper {
  std::map<std::string, Var> variables;

public:
  using MultiPoly = libpoly::MultiPoly;
  using UniPoly = libpoly::UniPoly;
  using Assignment = libpoly::Assignment;
  using RAN = libpoly::RAN;

  Var fresh_variable(const std::string name) {
    if (!variables.count(name)) {
      variables[name] = Var(name.c_str());
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
                        const std::vector<std::string> &var_name,
                        const std::vector<std::string> &values) {
    for (size_t i = 0; i < var_name.size(); ++i) {
      ass->set(fresh_variable(var_name[i]), build_RAN(values[i]));
    }
  }

  template <typename T> RAN build_RAN(const T &number) {
    // Build rational
    lp_algebraic_number_t temp;
    lp_algebraic_number_construct_from_rational(
        &temp, ((mpq_class)number).get_mpq_t());
    RAN ran = poly::AlgebraicNumber(&temp);
    lp_algebraic_number_destruct(&temp);
    return ran;
  }
};

} // namespace libpoly