#include <benchmark/benchmark.h>
#include <boost/format.hpp>

//TODO Still ugly
template <typename T>
static void printPolys(T poly1, T poly2, T result, std::string operation) {
  std::cout << boost::format("%-10s: (%s) %s (%s) = %-20s \n") % poly1.type() % poly1 % operation % poly2 % result;
}

template <typename T>
static void test_addition(benchmark::State& state, T& poly1, T& poly2) {
  for (auto _ : state) {
    // Perform method to test
    T res = poly1 + poly2;
  }
}

template <typename T>
static void test_addition(T& poly1, T& poly2) {
  printPolys(poly1, poly2, poly1 + poly2, "+");
}

template <typename T>
static void test_subtraction(benchmark::State& state, T& poly1, T& poly2) {
  for (auto _ : state) {
    // Perform method to test
    T res = poly1 - poly2;
  }
}

template <typename T>
static void test_subtraction(T& poly1, T& poly2) {
  printPolys(poly1, poly2, poly1 - poly2, "-");
}

template <typename T>
static void test_multiplication(benchmark::State& state, T& poly1, T& poly2) {
  for (auto _ : state) {
    // Perform method to test
    T res = poly1 * poly2;
  }
}

template <typename T>
static void test_multiplication(T& poly1, T& poly2) {
  printPolys(poly1, poly2, poly1 * poly2, "*");
}