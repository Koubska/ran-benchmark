#include <benchmark/benchmark.h>

template <typename T>
static void test_addition(benchmark::State &state, T &poly1, T &poly2,
                          bool debug = false) {
  for (auto _ : state) {
    // Perform method to test
    T res = poly1 + poly2;
  }
}

template <typename T>
static void test_subtraction(benchmark::State &state, T &poly1, T &poly2,
                             bool debug = false) {
  for (auto _ : state) {
    // Perform method to test
    T res = poly1 - poly2;
  }
}

template <typename T>
static void test_multiplication(benchmark::State &state, T &poly1, T &poly2,
                                bool debug = false) {
  for (auto _ : state) {
    // Perform method to test
    T res = poly1 * poly2;
  }
}
