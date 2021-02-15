#pragma once

#include "wrapper_carl.h"
#include "wrapper_libpoly.h"

class PolynomialGenerator {
public:
  static PolynomialGenerator &instance() {
    static PolynomialGenerator generator;
    return generator;
  }

  carl::MultiPoly &getCarlPoly1() { return poly_carl1; }

  carl::MultiPoly &getCarlPoly2() { return poly_carl2; }

  libpoly::MultiPoly &getLPPoly1() { return poly_libPoly1; }

  libpoly::MultiPoly &getLPPoly2() { return poly_libPoly2; }

private:
  carl::MultiPoly poly_carl1;
  carl::MultiPoly poly_carl2;

  libpoly::MultiPoly poly_libPoly1;
  libpoly::MultiPoly poly_libPoly2;

  template <typename T> auto createPoly1(T &t) { // y^2 - 1*z
    using P = typename T::MultiPoly;
    // Generate Variables
    auto y = t.fresh_variable("y");
    auto z = t.fresh_variable("z");

    // Generate Poly
    return P(1, y, 2) - P(1, z, 1);
  }

  template <typename T> auto createPoly2(T &t) { // 3x^2 + 2y
    using P = typename T::MultiPoly;
    // Generate Variables
    auto x = t.fresh_variable("x");
    auto y = t.fresh_variable("y");

    // Generate Poly
    return P(3, x, 2) + P(2, y, 1);
  }

  PolynomialGenerator() {
    // Create Wrapper
    carl::CarlWrapper carl_wrapper;
    libpoly::LibPolyWrapper libpoly_wrapper;
    // Create Polynomials
    poly_carl1 = createPoly1(carl_wrapper);
    poly_carl2 = createPoly2(carl_wrapper);
    poly_libPoly1 = createPoly1(libpoly_wrapper);
    poly_libPoly2 = createPoly2(libpoly_wrapper);

    std::cout << "Called PolyGeneratorConstructor" << std::endl;
    std::cout << "Carl:    " << poly_carl1 << "\t\t" << poly_carl2 << std::endl;
    std::cout << "LibPoly:    " << poly_libPoly1 << "\t\t" << poly_libPoly2
              << std::endl;
  }

  PolynomialGenerator(const PolynomialGenerator &);

  PolynomialGenerator &operator=(const PolynomialGenerator &);
};