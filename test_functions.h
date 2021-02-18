#include <benchmark/benchmark.h>
#include <boost/format.hpp>

//TODO Still ugly
template <typename T, typename C>
static void printPolys(T poly1, T poly2, C result, std::string operation) {
  std::cout << boost::format("%-15s: %15s (%s) (%s) = %-20s \n") % poly1.type() % operation % poly1  % poly2 % result;
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
  printPolys(poly1, poly2, poly1 + poly2, "Addition");
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
  printPolys(poly1, poly2, poly1 - poly2, "Subtraction");
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
  printPolys(poly1, poly2, poly1 * poly2 , "Multiplikation");
}


template <typename T, typename C>
static void test_resultant(benchmark::State& state, T& poly1, T& poly2, C& variable) {
  for (auto _ : state) {
    // Perform method to test
    auto res = resultant(poly1, poly2, variable) ;
  }
}


template <typename T, typename C>
static void test_resultant(T& poly1, T& poly2, C& variable) {
  printPolys(poly1, poly2, resultant(poly1, poly2, variable), "Resultant ");
}
