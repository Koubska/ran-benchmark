#define DO_BENCHMARK
#define WITH_CARL
#define WITH_LIBPOLY
//#define WITH_PARI

#ifdef WITH_CARL
#include "wrapper_carl.h"
#endif 
#ifdef WITH_LIBPOLY
#include "wrapper_lp.h"
#endif
#ifdef WITH_PARI
#include "wrapper_pari.h"
#endif

//#include "wrapper_gap.h"
#include "benchmark/benchmark.h"

#include <array>
#include <cstddef>
#include <iostream>

template<typename W>
void perform_test(W& w, bool print_output = false) {
    using P = typename W::Poly;
    auto x = w.fresh_variable("x");
    auto y = w.fresh_variable("y");
    auto z = w.fresh_variable("z");
    
    w.set_variable(x, {-2, 0, 1}, 1, 2);
    w.set_variable(y, {-2, 0, 0, 0, 1}, 1, 2);

    auto p = (P(1, y, 2) + P(1, x, 1)) - P(1, z, 1);

    auto samples = w.lift(p, z);
    if (print_output) {
        for (const auto& s: samples) {
            std::cout << "-> " << s << std::endl;
        }
    }
    samples.clear();
}

#ifdef WITH_CARL
static void BM_carl(benchmark::State& state) {
  for (auto _ : state) {
    carl::CarlWrapper wrapper;
    perform_test(wrapper);
  }
}
BENCHMARK(BM_carl);
#endif

#ifdef WITH_LIBPOLY
static void BM_libpoly(benchmark::State& state) {
  for (auto _ : state) {
    lp::LibPolyWrapper wrapper;
    perform_test(wrapper);
  }
}
BENCHMARK(BM_libpoly);
#endif

#ifdef WITH_PARI
static void BM_pari(benchmark::State& state) {
  for (auto _ : state) {
    pari::PariWrapper wrapper;
    perform_test(wrapper);
  }
}
BENCHMARK(BM_pari);
#endif

#ifdef DO_BENCHMARK

BENCHMARK_MAIN();

#else

int main() {
#ifdef WITH_CARL
    {
        std::cout << "***** CArL *****" << std::endl;
        carl::CarlWrapper wrapper;
        perform_test(wrapper, true);
    }
#endif
#ifdef WITH_LIBPOLY
    {
        std::cout << "***** libpoly *****" << std::endl;
        lp::LibPolyWrapper wrapper;
        perform_test(wrapper, true);
    }
#endif
#ifdef WITH_PARI
    {
        std::cout << "***** PARI *****" << std::endl;
        pari::PariWrapper wrapper;
        perform_test(wrapper, true);
    }
#endif
}

#endif
