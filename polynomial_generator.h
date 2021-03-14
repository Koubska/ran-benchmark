#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <sstream>

#include "../carl/src/carl/core/UnivariatePolynomial.h"
#include "../libpoly/include/assignment.h"
#include "wrapper_carl.h"
#include "wrapper_libpoly.h"


#include "../carl/src/tests/benchmarks/framework/BenchmarkGenerator.h" //To generate random polynomials

class PolynomialGenerator {
public:
  static PolynomialGenerator &instance() {
    static PolynomialGenerator generator;
    return generator;
  }

  auto &getCarlPoly1() { return poly_carl1; }

  auto &getCarlPoly2() { return poly_carl2; }

  auto &getCarlPoly1UniMulti() { return poly_carl1_um; }

  auto &getCarlPoly2UniMulti() { return poly_carl2_um; }

  auto &getLPPoly1() { return poly_libpoly1; }

  auto &getLPPoly2() { return poly_libpoly2; }

  auto &getCarlVariable() { return var_carl; }

  auto &getLPVariable() { return var_libpoly; }

  auto &getCarlAssignment() { return *assignment_carl; }

  auto &getLPAssignment() { return *assignment_libpoly; }

private:
  carl::MultiPoly poly_carl1;
  carl::MultiPoly poly_carl2;
  carl::UniMultiPoly poly_carl1_um;
  carl::UniMultiPoly poly_carl2_um;

  carl::Var var_carl;
  std::shared_ptr<carl::Assignment> assignment_carl =
      std::shared_ptr<carl::Assignment>(new carl::Assignment());

  libpoly::MultiPoly poly_libpoly1;
  libpoly::MultiPoly poly_libpoly2;
  libpoly::Var var_libpoly;
  std::shared_ptr<libpoly::Assignment> assignment_libpoly =
      std::shared_ptr<libpoly::Assignment>(new libpoly::Assignment());

  inline bool isInteger(const std::string &s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
      return false;
    }
    char *p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
  }

  template <typename T> auto createMultiPoly(T &t, std::string input) {
    using P = typename T::MultiPoly;
    // string cleanup
    input.erase(remove_if(input.begin(), input.end(), isspace), input.end());

    // split by + to get each monomial
    std::vector<std::string> monomials;
    boost::split(monomials, input, boost::is_any_of("+"));

    auto poly = P(0);  // Zero Poly
    auto monom = P(1); // Used for construction of poly
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

  template <typename T, typename C> auto createAssignment(T &t, C ass) {
    // Insertions need to be in the same order
    std::vector<std::string> var_names;
    std::vector<std::string> var_assignments;
    var_names.emplace_back("x");
    var_assignments.emplace_back("123");

    var_names.emplace_back("y");
    var_assignments.emplace_back("321");

    return t.build_assignment(ass, var_names, var_assignments);
  }

  template <typename T, typename C> auto createRAN(T &t, C &number) {
    return t.build_RAN(number);
  }

  PolynomialGenerator() {
    carl::CarlWrapper carl_wrapper;
    libpoly::LibPolyWrapper libpoly_wrapper;

    
    size_t amount_variables = 2 ;
    carl::BenchmarkInformation benchmark_info(carl::BenchmarkSelection::Random, amount_variables) ;
    benchmark_info.degree = 20 ; // Max degree of monomials 

    //Todo limit in amount of monomials
    //Dirty aber was solls, ist für die benchmarks egal
    carl::ObjectGenerator generator(benchmark_info) ;
    auto pol1 = generator.newMP<mpq_class>() ;
    auto pol2 = generator.newMP<mpq_class>() ;
    auto var = generator.randomVariable() ;

    std::ostringstream pol1_stream ;
    pol1_stream << pol1 ;
    std::ostringstream pol2_stream ;
    pol2_stream << pol2 ;
    std::ostringstream var_stream ;
    var_stream << var ; 

    // Create Polynomials
    //std::string poly1 = "x+y";
    //std::string poly2 = "y+ -1 *x";
    //std::string main_variable = "y";
    std::string poly1 = pol1_stream.str() ;
    std::string poly2 = pol2_stream.str() ;
    std::string main_variable = var_stream.str() ;


    poly_carl1 = createMultiPoly(carl_wrapper, poly1);
    poly_carl2 = createMultiPoly(carl_wrapper, poly2);
    var_carl = carl_wrapper.fresh_variable(main_variable);
    poly_carl1_um = carl::to_univariate_polynomial(poly_carl1.poly(), var_carl);
    poly_carl2_um = carl::to_univariate_polynomial(poly_carl2.poly(), var_carl);

    poly_libpoly1 = createMultiPoly(libpoly_wrapper, poly1);
    poly_libpoly2 = createMultiPoly(libpoly_wrapper, poly2);
    var_libpoly = libpoly_wrapper.fresh_variable(main_variable);

    std::cout << "Carl Polys: " << poly_carl1.poly() << "\t\t"
              << poly_carl2.poly() << std::endl;
    std::cout << "Carl Variable: " << var_carl << std::endl ;
    std::cout << "LibPoly Polys: " << poly_libpoly1.poly() << "\t\t"
              << poly_libpoly2.poly() << std::endl;
    std::cout << "LibPoly Variable: " << var_libpoly << std::endl ;


    createAssignment(carl_wrapper, assignment_carl);
    std::cout << "Carl Assignment: " << *assignment_carl << std::endl;

    createAssignment(libpoly_wrapper, assignment_libpoly);
    std::cout << "LibPoly Assignment: " << *assignment_libpoly << std::endl;
  }

  PolynomialGenerator(const PolynomialGenerator &);

  PolynomialGenerator &operator=(const PolynomialGenerator &);
};