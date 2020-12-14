#include <benchmark/benchmark.h>
#include "polynomial_generator.h"


template <typename T>
static void perform_test(benchmark::State& state, T& poly1, T& poly2) {    
    T res ;
    for (auto _ : state){
      //Perform method to test
      res = poly1 + poly2 ;
    } 
    std::cout << res << std::endl ; 
  }

