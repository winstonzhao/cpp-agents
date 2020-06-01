#include "benchmark/benchmark.h"
#include <iostream>

static void BM_TEST(benchmark::State &state)
{
  while (state.KeepRunning())
  {
    std::cout << state.range(0) << std::endl;
  }
}

BENCHMARK(BM_TEST)
    ->Args({100});

BENCHMARK_MAIN();
