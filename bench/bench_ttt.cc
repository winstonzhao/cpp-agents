#include <benchmark/benchmark.h>
#include <iostream>

#include "gyms/mmmc_ttt_gym.h"
#include "gyms/mmsl_ttt_gym.h"

using namespace CppAgents;

static void MMMC_TTT(benchmark::State &state)
{
  MmmcTttGym::MmmcTttGym<3> gym{(double)state.range(0) / 10, (double)state.range(1) / 10};
  while (state.KeepRunning())
  {
    const auto res = gym.TrainAndTest(state.range(2));
    state.counters.insert({{"Timesteps", res.first}, {"Optimality", res.second}});
  }
}

static void MMSL_TTT(benchmark::State &state)
{
  MmslTttGym::MmslTttGym<3> gym{
      (double)state.range(0) / 10,
      (double)state.range(1) / 10,
      (double)state.range(2) / 10,
      (double)state.range(3) / 10};
  while (state.KeepRunning())
  {
    const auto res = gym.TrainAndTest(state.range(4));
    state.counters.insert({{"Timesteps", res.first}, {"Optimality", res.second}});
  }
}

// // Find Epsilon
BENCHMARK(MMSL_TTT)
    ->Args({1, 1, 5, 5, 100000})
    ->Args({1, 2, 5, 5, 100000})
    ->Args({1, 3, 5, 5, 100000})
    ->Args({1, 4, 5, 5, 100000})
    ->Args({1, 5, 5, 5, 100000})
    ->Args({1, 6, 5, 5, 100000})
    ->Args({1, 7, 5, 5, 100000})
    ->Args({1, 8, 5, 5, 100000})
    ->Args({1, 9, 5, 5, 100000})
    ->Args({1, 10, 5, 5, 100000});

// // Find Lambda
// BENCHMARK(MMSL_TTT)
//     ->Args({1, 8, 1, 5, 100000})
//     ->Args({1, 8, 2, 5, 100000})
//     ->Args({1, 8, 3, 5, 100000})
//     ->Args({1, 8, 4, 5, 100000})
//     ->Args({1, 8, 5, 5, 100000})
//     ->Args({1, 8, 6, 5, 100000})
//     ->Args({1, 8, 7, 5, 100000})
//     ->Args({1, 8, 8, 5, 100000})
//     ->Args({1, 8, 9, 5, 100000})
//     ->Args({1, 8, 10, 5, 100000});

// // Find Gamma
// BENCHMARK(MMSL_TTT)
//     ->Args({1, 8, 8, 0, 100000})
//     ->Args({1, 8, 8, 1, 100000})
//     ->Args({1, 8, 8, 2, 100000})
//     ->Args({1, 8, 8, 3, 100000})
//     ->Args({1, 8, 8, 4, 100000})
//     ->Args({1, 8, 8, 5, 100000})
//     ->Args({1, 8, 8, 6, 100000})
//     ->Args({1, 8, 8, 7, 100000})
//     ->Args({1, 8, 8, 8, 100000})
//     ->Args({1, 8, 8, 9, 100000})
//     ->Args({1, 8, 8, 10, 100000});

// BENCHMARK(MMSL_TTT)
//     ->Args({1, 8, 8, 5, 100})
//     ->Args({1, 8, 8, 5, 1000})
//     ->Args({1, 8, 8, 5, 10000})
//     ->Args({1, 8, 8, 5, 100000})
//     ->Args({1, 8, 8, 5, 1000000})
//     ->Args({1, 8, 8, 5, 10000000})
//     ->Args({1, 8, 8, 5, 100000000})
//     ->Args({1, 8, 8, 5, 1000000000});

BENCHMARK(MMMC_TTT)
    ->Args({1, 5, 100000})
    ->Args({2, 5, 100000})
    ->Args({3, 5, 100000})
    ->Args({4, 5, 100000})
    ->Args({5, 5, 100000})
    ->Args({6, 5, 100000})
    ->Args({7, 5, 100000})
    ->Args({8, 5, 100000})
    ->Args({9, 5, 100000})
    ->Args({10, 5, 100000});

BENCHMARK_MAIN();
