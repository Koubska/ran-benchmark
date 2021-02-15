#define WITH_CARL
#define WITH_LIBPOLY

static const bool DEBUG = false;

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



#ifdef WITH_CARL
BENCHMARK_CAPTURE(test_addition, CARL,
                  PolynomialGenerator::instance().getCarlPoly1(),
                  PolynomialGenerator::instance().getCarlPoly2(), DEBUG);
BENCHMARK_CAPTURE(test_subtraction, CARL,
                  PolynomialGenerator::instance().getCarlPoly1(),
                  PolynomialGenerator::instance().getCarlPoly2(), DEBUG);
BENCHMARK_CAPTURE(test_multiplication, CARL,
                  PolynomialGenerator::instance().getCarlPoly1(),
                  PolynomialGenerator::instance().getCarlPoly2(), DEBUG);
#endif

#ifdef WITH_LIBPOLY
BENCHMARK_CAPTURE(test_addition, LIBPOLY,
                  PolynomialGenerator::instance().getLPPoly1(),
                  PolynomialGenerator::instance().getLPPoly2(), DEBUG);
BENCHMARK_CAPTURE(test_subtraction, LIBPOLY,
                  PolynomialGenerator::instance().getLPPoly1(),
                  PolynomialGenerator::instance().getLPPoly2(), DEBUG);
BENCHMARK_CAPTURE(test_multiplication, LIBPOLY,
                  PolynomialGenerator::instance().getLPPoly1(),
                  PolynomialGenerator::instance().getLPPoly2(), DEBUG);

#endif

BENCHMARK_MAIN();
