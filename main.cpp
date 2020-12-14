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
#include "test_functions.h"
#include "polynomial_generator.h"
#include <array>
#include <iostream> 

#ifdef WITH_CARL
BENCHMARK_CAPTURE(perform_test, CARL, PolynomialGenerator::instance().getCarlPoly1(), PolynomialGenerator::instance().getCarlPoly2());
#endif

#ifdef WITH_LIBPOLY
BENCHMARK_CAPTURE(perform_test, LIBPOLY, PolynomialGenerator::instance().getLPPoly1(), PolynomialGenerator::instance().getLPPoly2());
#endif

#ifdef WITH_PARI
static void BM_pari(benchmark::State& state) {
    pari::PariWrapper wrapper;
    test_functions::perform_test(wrapper, state);
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
        perform_test(wrapper, nullptr, true);
    }
#endif
#ifdef WITH_LIBPOLY
    {
        std::cout << "***** libpoly *****" << std::endl;
        lp::LibPolyWrapper wrapper;
        perform_test(wrapper, nullptr, true);
    }
#endif
#ifdef WITH_PARI
    {
        std::cout << "***** PARI *****" << std::endl;
        pari::PariWrapper wrapper;
        perform_test(wrapper, nullptr, true);
    }
#endif
}

#endif
