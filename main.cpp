#define WITH_CARL
#define WITH_LIBPOLY

#define DEBUG

#ifdef WITH_CARL
#include "wrapper_carl.h"
#endif
#ifdef WITH_LIBPOLY
#include "wrapper_libpoly.h"
#endif

#include <array>
#include <iostream>

#include "benchmark/benchmark.h"
#include "polynomial_generator.h"
#include "test_functions.h"

#ifndef DEBUG

#ifdef WITH_CARL
BENCHMARK_CAPTURE(test_addition,
                  CARL,
                  PolynomialGenerator::instance().getCarlPoly1(),
                  PolynomialGenerator::instance().getCarlPoly2());
BENCHMARK_CAPTURE(test_subtraction,
                  CARL,
                  PolynomialGenerator::instance().getCarlPoly1(),
                  PolynomialGenerator::instance().getCarlPoly2());
BENCHMARK_CAPTURE(test_multiplication,
                  CARL,
                  PolynomialGenerator::instance().getCarlPoly1(),
                  PolynomialGenerator::instance().getCarlPoly2());

BENCHMARK_CAPTURE(test_resultant,
                  CARL,
                  PolynomialGenerator::instance().getCarlPoly1(),
                  PolynomialGenerator::instance().getCarlPoly2(),
                  PolynomialGenerator::instance().getCarlVariable());
#endif

#ifdef WITH_LIBPOLY
BENCHMARK_CAPTURE(test_addition,
                  LIBPOLY,
                  PolynomialGenerator::instance().getLPPoly1(),
                  PolynomialGenerator::instance().getLPPoly2());
BENCHMARK_CAPTURE(test_subtraction,
                  LIBPOLY,
                  PolynomialGenerator::instance().getLPPoly1(),
                  PolynomialGenerator::instance().getLPPoly2());
BENCHMARK_CAPTURE(test_multiplication,
                  LIBPOLY,
                  PolynomialGenerator::instance().getLPPoly1(),
                  PolynomialGenerator::instance().getLPPoly2());

BENCHMARK_CAPTURE(test_resultant,
                  LIBPOLY,
                  PolynomialGenerator::instance().getLPPoly1(),
                  PolynomialGenerator::instance().getLPPoly2(),
                  PolynomialGenerator::instance().getLibPolyVariable());

#endif

BENCHMARK_MAIN();

#else

int main(int argc, char const* argv[]) {
  std::cout << "DEBUG MODE" << std::endl;

  test_addition(PolynomialGenerator::instance().getCarlPoly1(),
                PolynomialGenerator::instance().getCarlPoly2());
  test_addition(PolynomialGenerator::instance().getLPPoly1(),
                PolynomialGenerator::instance().getLPPoly2());

  test_subtraction(PolynomialGenerator::instance().getCarlPoly1(),
                   PolynomialGenerator::instance().getCarlPoly2());
  test_subtraction(PolynomialGenerator::instance().getLPPoly1(),
                   PolynomialGenerator::instance().getLPPoly2());

  test_multiplication(PolynomialGenerator::instance().getCarlPoly1(),
                      PolynomialGenerator::instance().getCarlPoly2());
  test_multiplication(PolynomialGenerator::instance().getLPPoly1(),
                      PolynomialGenerator::instance().getLPPoly2());

  test_resultant(PolynomialGenerator::instance().getCarlPoly1(),
                 PolynomialGenerator::instance().getCarlPoly2(),
                 PolynomialGenerator::instance().getCarlVariable());
  test_resultant(PolynomialGenerator::instance().getLPPoly1(),
                 PolynomialGenerator::instance().getLPPoly2(),
                 PolynomialGenerator::instance().getLibPolyVariable());

  return 0;
}

#endif