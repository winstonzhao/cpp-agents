#include <benchmark/benchmark.h>
#include <iostream>

#include "gyms/mmmc_ttt_gym.h"

using namespace CppAgents;

static void BENCHMARK_TRAINING(benchmark::State &state)
{
  MmmcTttGym::MmmcTttGym<3> gym{0.1, 0.4};
  while (state.KeepRunning())
  {
    const auto res = gym.TrainAndTest(state.range(0));
    state.counters.insert({{"Foo", numFoos}, {"Bar", numBars}});
  }
}

BENCHMARK(BENCHMARK_TRAINING)
    ->Args({10});

BENCHMARK_MAIN();
