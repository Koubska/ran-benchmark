#include <benchmark/benchmark.h>

template <typename T>
static void test_addition(benchmark::State &state, T &poly1, T &poly2,
                          bool debug) {
  if (debug) {
    std::cout << poly1 << " + " << poly2 << " = " << poly1 + poly2 << std::endl;
    state.SkipWithError("Debug mode");
  } else {
    for (auto _ : state) {
      // Perform method to test
      T res = poly1 + poly2;
    }
  }
}

template <typename T>
static void test_subtraction(benchmark::State &state, T &poly1, T &poly2,
                             bool debug) {
  if (debug) {
    std::cout << poly1 << " - " << poly2 << " = " << poly1 - poly2 << std::endl;
    state.SkipWithError("Debug mode");
  } else {
    for (auto _ : state) {
      // Perform method to test
      T res = poly1 - poly2;
    }
  }
}

template <typename T>
static void test_multiplication(benchmark::State &state, T &poly1, T &poly2,
                                bool debug) {
  if (debug) {
    std::cout << poly1 << " * " << poly2 << " = " << poly1 * poly2 << std::endl;
    state.SkipWithError("Debug mode");
  } else {
    for (auto _ : state) {
      // Perform method to test
      T res = poly1 * poly2;
    }
  }
}