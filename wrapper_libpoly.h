#pragma once

/*
 * Wrapper for libpoly. Download and build libpoly somewhere and adapt the path
 * here.
 */

#include <iostream>
#include <map>
#include <string>

#include "../libpoly/include/poly.h"
#include "../libpoly/include/polynomial.h"
#include "../libpoly/include/polynomial_context.h"
#include "../libpoly/include/polyxx.h"
#include "../libpoly/include/variable_order.h"
#include "../libpoly/include/algebraic_number.h"

namespace libpoly {

using UPoly = poly::UPolynomial;
using MPoly = poly::Polynomial;
using Var = poly::Variable;
using RAN = poly::AlgebraicNumber ;
using Assignment = poly::Assignment ;

class UniPoly {
  friend std::ostream& operator<<(std::ostream& os, const UniPoly& p);

 private:
  UPoly uPoly;

 public:
  UniPoly(int constant)
      : uPoly(poly::Integer(constant)) {}            // Create Constant Poly
  UniPoly(UPoly&& poly) : uPoly(std::move(poly)) {}  // Copy Constructor
  UniPoly(int coeff, Var x, unsigned int pow)
      : uPoly(pow, coeff) {}  // coeff * x^pow TODO var is not used

  const std::string type() const { return "LibPoly Uni"; }

  const UPoly poly() const { return uPoly; }
  UPoly poly() { return uPoly; }
};

class MultiPoly {
  friend std::ostream& operator<<(std::ostream& os, const MultiPoly& p);

 private:
  MPoly mPoly;

 public:
  MultiPoly() : mPoly() {}  // Construct Zero Polynomial
  MultiPoly(int constant)
      : mPoly(poly::Integer(constant)) {}              // Create Constant Poly
  MultiPoly(MPoly&& poly) : mPoly(std::move(poly)) {}  // Copy Constructor
  MultiPoly(int coeff, Var var, unsigned int pow)
      : mPoly(poly::Integer(coeff), var, pow) {}

  const std::string type() const { return "LibPoly Multi"; }

  const MPoly poly() const { return mPoly; }
  MPoly poly() { return mPoly; }
};

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

auto resultant(const MultiPoly& lhs, const MultiPoly& rhs, const Var& mainVar) {
  // lhs.poly().get_internal()->ctx->var_order->top = mainVar.get_internal();
  // rhs.poly().get_internal()->ctx->var_order->top = mainVar.get_internal();
  // auto temp = lhs.poly().get_internal();
  // auto temp2 = lp_polynomial_get_context(temp);
  // auto temp3 = temp2->var_order;
  //std::cout << poly::main_variable(lhs.poly()) << std::endl ; 
  //std::cout << poly::main_variable(rhs.poly()) << std::endl ; 

  //TODO still unclear how to set the main variable
  return poly::resultant(lhs.poly(), rhs.poly());
}

class LibPolyWrapper {
  std::map<std::string, libpoly::Var> variables;

 public:
  using MultiPoly = libpoly::MultiPoly;
  using UniPoly = libpoly::UniPoly;
  using Assignment = libpoly::Assignment ;
  using RAN = libpoly::RAN ;

  Var fresh_variable(const std::string name) {
    if (variables.count(name)) {
      return variables[name];
    } else {
      auto temp = Var(name.c_str());
      variables[name] = temp;
      return temp;
    }
  }

  std::map<std::string, Var>& get_variables(){
    return variables; 
  }
  
  std::vector<std::string> get_variable_names(){
    std::vector<std::string> names ;
    for(const auto& items : variables){
      names.push_back(items.first) ;
    }
    return names ; 
  }

  //Assumes var_name.size() == values.size() 
  Assignment build_assignment(std::vector<std::string> var_name, std::vector<RAN> values){
    Assignment ass ; 
    for(size_t i = 0; i < var_name.size(); ++i){
      ass.set(variables[var_name[i]], poly::Value(values[i])) ;
    }
    std::cout << "In builder: " << ass << std::endl ; 
    return ass ;
  }

  template <typename T>
  RAN build_RAN(T number){
    //Build rational
    poly::Rational rat = poly::Rational((mpq_class)number) ;
    RAN ran ; 
    lp_algebraic_number_construct_from_rational(ran.get_internal(), rat.get_internal()) ;
    return ran ; 
  }


};

}  // namespace libpoly