#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <string>
#include <vector>

#include "wrapper_carl.h"
#include "wrapper_libpoly.h"

class PolynomialGenerator {
 public:
  static PolynomialGenerator& instance() {
    static PolynomialGenerator generator;
    return generator;
  }

  auto& getCarlPoly1() { return poly_carl1; }

  auto& getCarlPoly2() { return poly_carl2; }

  auto& getLPPoly1() { return poly_libPoly1; }

  auto& getLPPoly2() { return poly_libPoly2; }

  auto& getCarlVariable() { return var_carl; }

  auto& getLibPolyVariable() { return var_libpoly; }

 private:
  carl::MultiPoly poly_carl1;
  carl::MultiPoly poly_carl2;
  carl::Var var_carl;

  libpoly::MultiPoly poly_libPoly1;
  libpoly::MultiPoly poly_libPoly2;
  libpoly::Var var_libpoly;

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

  PolynomialGenerator() {
    carl::CarlWrapper carl_wrapper;
    libpoly::LibPolyWrapper libpoly_wrapper;
    // Create Polynomials

    std::string poly1 = "-1*x^5 + -3*x^3 + - 6" ;
    std::string poly2 = "x^5 + -1 * x^3 + 10*x" ;
    std::string variable = "x" ;


    poly_carl1 = createMultiPoly(carl_wrapper, poly1);
    poly_carl2 = createMultiPoly(carl_wrapper, poly2);
    poly_libPoly1 = createMultiPoly(libpoly_wrapper, poly1);
    poly_libPoly2 = createMultiPoly(libpoly_wrapper, poly2);

    var_carl = carl_wrapper.fresh_variable(variable);
    var_libpoly = libpoly_wrapper.fresh_variable(variable);
  }

  PolynomialGenerator(const PolynomialGenerator&);

  PolynomialGenerator& operator=(const PolynomialGenerator&);
};