#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <string>
#include <vector>

#include "wrapper_carl.h"
#include "wrapper_libpoly.h"
#include "../libpoly/include/assignment.h"

class PolynomialGenerator {
 public:
  static PolynomialGenerator& instance() {
    static PolynomialGenerator generator;
    return generator;
  }

  auto& getCarlPoly1() { return poly_carl1; }

  auto& getCarlPoly2() { return poly_carl2; }

  auto& getLPPoly1() { return poly_libpoly1; }

  auto& getLPPoly2() { return poly_libpoly2; }

  auto& getCarlVariable() { return var_carl; }

  auto& getLPVariable() { return var_libpoly; }

  auto& getCarlAssignment() { return assignment_carl; }

  auto& getLPAssignment() { return assignment_libpoly; }

 private:
  carl::MultiPoly poly_carl1;
  carl::MultiPoly poly_carl2;
  carl::Var var_carl;
  carl::Assignment assignment_carl;

  libpoly::MultiPoly poly_libpoly1;
  libpoly::MultiPoly poly_libpoly2;
  libpoly::Var var_libpoly;
  libpoly::Assignment assignment_libpoly;

  inline bool isInteger(const std::string& s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
      return false;
    }
    char* p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
  }

  template <typename T>
  auto createMultiPoly(T& t, std::string input) {
    using P = typename T::MultiPoly;
    // string cleanup
    input.erase(remove_if(input.begin(), input.end(), isspace), input.end());

    // split by + to get each monomial
    std::vector<std::string> monomials;
    boost::split(monomials, input, boost::is_any_of("+"));

    auto poly = P(0);   // Zero Poly
    auto monom = P(1);  // Used for construction of poly
    // Iterate over each monomial
    for (std::string monomial : monomials) {
      // Split by * to get each product
      std::vector<std::string> prods;
      boost::split(prods, monomial, boost::is_any_of("*"));

      monom = P(1);
      // Iterate over products to build the monomial
      for (std::string prod : prods) {
        if (isInteger(prod)) {
          // Is Constant
          monom = monom * P(std::stoi(prod));
        } else if (prod.find("^") != std::string::npos) {
          // Is Power var^pow
          std::vector<std::string> temp;
          boost::split(temp, prod, boost::is_any_of("^"));
          auto var = t.fresh_variable(temp[0]);
          auto pow = std::stoi(temp[1]);
          monom = monom * P(1, var, pow);
        } else {
          // Is Variable
          auto var = t.fresh_variable(prod);
          monom = monom * P(var);
        }
      }
      poly = poly + monom;
    }
    return poly;
  }

  template <typename T>
  auto createAssignment(T& t) {
    // Insertions needs to be in the same order
    using RAN = typename T::RAN ;
    std::vector<std::string> var_names ;
    std::vector<RAN> var_assignments ;
    var_names.emplace_back("y") ;
    var_assignments.emplace_back(t.build_RAN(123) ) ;

    auto ass = t.build_assignment(var_names, var_assignments);
    std::cout << ass << std::endl;
    return ass;
  }

  PolynomialGenerator() {
    carl::CarlWrapper carl_wrapper;
    libpoly::LibPolyWrapper libpoly_wrapper;
    // Create Polynomials

    std::string poly1 = "-1*x^5 + -3*y^3 + - 6";
    std::string poly2 = "x^5 + -1 * y^3 + 10*x";
    std::string main_variable = "y";

    poly_carl1 = createMultiPoly(carl_wrapper, poly1);
    poly_carl2 = createMultiPoly(carl_wrapper, poly2);
    poly_libpoly1 = createMultiPoly(libpoly_wrapper, poly1);
    poly_libpoly2 = createMultiPoly(libpoly_wrapper, poly2);

    var_carl = carl_wrapper.fresh_variable(main_variable);
    var_libpoly = libpoly_wrapper.fresh_variable(main_variable);

    assignment_carl = createAssignment(carl_wrapper) ;
    assignment_libpoly = createAssignment(libpoly_wrapper) ; 
    std::cout << "In Function: " << assignment_libpoly << std::endl ; 
  }

  PolynomialGenerator(const PolynomialGenerator&);

  PolynomialGenerator& operator=(const PolynomialGenerator&);

  ~PolynomialGenerator(){
    std::cout << "Called Destructor BEFORE" << std::endl ;
    std::cout << lp_assignment_to_string(assignment_libpoly.get_internal()) << std::endl ; 
    assignment_libpoly.unset(var_libpoly) ;
    std::cout << assignment_libpoly << std::endl ; 
    std::cout << "Called Destructor AFTER" << std::endl ;
  }
};