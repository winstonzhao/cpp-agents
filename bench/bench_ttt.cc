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
    state.counters.insert({{"Timesteps", res.timesteps}, {"Optimality", res.optimality}});
  }
}

static void MMMC_LOSS_TTT(benchmark::State &state)
{
  MmmcTttGym::MmmcTttGym<3> gym{0.1, 0.01};
  while (state.KeepRunning())
  {
    for (int i = 0; i < state.range(0); i++)
    {
      const auto res = gym.TrainAndTest(state.range(1));
      std::cout << res.timesteps << ":" << res.optimality << ":" << res.loss << std::endl;
    }
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
    state.counters.insert({{"Timesteps", res.timesteps}, {"Optimality", res.optimality}});
  }
}

static void MMSL_LOSS_TTT(benchmark::State &state)
{
  MmslTttGym::MmslTttGym<3> gym{0.05, 0.4, 1, 0.5};
  while (state.KeepRunning())
  {
    for (int i = 0; i < state.range(0); i++)
    {
      const auto res = gym.TrainAndTest(state.range(1));
      std::cout << res.timesteps << ":" << res.optimality << ":" << res.loss << std::endl;
    }
  }
}

BENCHMARK(MMSL_LOSS_TTT)
    ->Args({100, 10000000});

BENCHMARK(MMMC_LOSS_TTT)
    ->Args({100, 10000000});

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
