#include <benchmark/benchmark.h>
#include <iostream>

#include "gyms/mmmc_ttt_gym.h"

using namespace CppAgents::MmmcTttGym;

static void BENCHMARK_TRAINING(benchmark::State &state)
{
  while (state.KeepRunning())
  {
    TrainAndTest(state.range(0));
  }
}

BENCHMARK(BENCHMARK_TRAINING)
    ->Args({10});

BENCHMARK_MAIN();
