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

 private:
  carl::MultiPoly poly_carl1;
  carl::MultiPoly poly_carl2;

  libpoly::MultiPoly poly_libPoly1;
  libpoly::MultiPoly poly_libPoly2;

  inline bool isInteger(const std::string& s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
      return false;
    }
    char* p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
  }

  template <typename T>
  auto createPoly1(T& t) {  // y^2 - 1*z
    using P = typename T::MultiPoly;
    // Generate Variables
    auto y = t.fresh_variable("y");
    auto z = t.fresh_variable("z");

    // Generate Poly
    return P(1, y, 2) - P(1, z, 1);
  }

  template <typename T>
  auto createPoly2(T& t) {  // 3x^2 + 2y
    using P = typename T::MultiPoly;
    // Generate Variables
    auto x = t.fresh_variable("x");
    auto y = t.fresh_variable("y");

    // Generate Poly
    return P(3, x, 2) + P(2, y, 1);
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
    // Create Wrapper
    carl::CarlWrapper carl_wrapper;
    libpoly::LibPolyWrapper libpoly_wrapper;
    // Create Polynomials
    poly_carl1 = createMultiPoly(carl_wrapper, "-1*x1^2 + -3");
    poly_carl2 = createMultiPoly(carl_wrapper, "-1*x1");
    poly_libPoly1 = createMultiPoly(libpoly_wrapper, "-1*x1^2 + -3");
    poly_libPoly2 = createMultiPoly(libpoly_wrapper, "-1*x1");
  }

  PolynomialGenerator(const PolynomialGenerator&);

  PolynomialGenerator& operator=(const PolynomialGenerator&);
};