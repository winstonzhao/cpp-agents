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

BENCHMARK(MMSL_TTT)
    ->Args({1, 8, 8, 5, 100000 << 0})
    ->Args({1, 8, 8, 5, 100000 << 1})
    ->Args({1, 8, 8, 5, 100000 << 2})
    ->Args({1, 8, 8, 5, 100000 << 3})
    ->Args({1, 8, 8, 5, 100000 << 4})
    ->Args({1, 8, 8, 5, 100000 << 5})
    ->Args({1, 8, 8, 5, 100000 << 6})
    ->Args({1, 8, 8, 5, 100000 << 7});

BENCHMARK(MMMC_TTT)
    ->Args({2, 5, 100000 << 0})
    ->Args({2, 5, 100000 << 1})
    ->Args({2, 5, 100000 << 2})
    ->Args({2, 5, 100000 << 3})
    ->Args({2, 5, 100000 << 4})
    ->Args({2, 5, 100000 << 5})
    ->Args({2, 5, 100000 << 6})
    ->Args({2, 5, 100000 << 7});
