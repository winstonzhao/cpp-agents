#include <iostream>
#include <string>
#include <algorithm>

#include "agent/MMMCAgent.h"
#include "environment/TttEnvironment.h"
#include "trajectory/Trajectory.h"
#include "util/RandomProvider.h"

auto main(int argc, char **argv) -> int
{
  using namespace CppAgents::Environment::TttEnvironment;
  using namespace CppAgents::Agent;
  using namespace CppAgents::Trajectory;

  constexpr int LENGTH = 3;
  constexpr int ARRAY_SIZE = LENGTH * LENGTH + 1;

  using env_t = TttEnvironment<LENGTH>;
  using ts_t = TimeStepType<std::array<int, ARRAY_SIZE>, double, int>;
  using traj_t = Trajectory<ts_t, int>;

  env_t env;
  MMMCAgent<traj_t> agent{env_t::GetActions, env_t::IsMax};

  auto train = [&](int numEpisodes) {
    int episodes = 0;
    double sumLoss = 0;
    std::vector<traj_t> data;

    while (episodes < numEpisodes)
    {
      ts_t next, prev;
      next.stepType = FIRST;
      auto policy = agent.GetCollectPolicy();
      decltype(policy)::policystep_t res;
      while (next.stepType != LAST)
      {
        prev = env.GetCurrentTimeStep();
        res = policy.Action(prev);
        next = env.Step(res.action);
        data.emplace_back(traj_t{prev, res.action, next});
      }

      agent.SetEpsilon(res.info);

      auto loss = agent.Train(data);
      sumLoss += std::abs(loss);
      episodes++;
      data.clear();

      // if (episodes % 500 == 0)
      // {
      //   printf("Episode: %d, Average Loss: %f\n", episodes, sumLoss / 500);
      //   sumLoss = 0;
      // }
    };
  };

  auto test = [&](int numEpisodes) {
    int episodes = 0;
    double sumLoss = 0;
    std::vector<traj_t> data;

    while (episodes < numEpisodes)
    {
      ts_t next, prev;
      next.stepType = FIRST;
      auto policy = agent.GetPolicy();
      decltype(policy)::policystep_t res;
      while (next.stepType != LAST)
      {
        prev = env.GetCurrentTimeStep();
        res = policy.Action(prev);
        next = env.Step(res.action);
        data.emplace_back(traj_t{prev, res.action, next});
      }

      auto loss = agent.Train(data);
      sumLoss += std::abs(loss);
      episodes++;
      data.clear();
    }

    printf("Testing Loss: %f\n", sumLoss / 500);
  };

  while (true)
  {
    train(1000);
    test(100);
  }

  return 0;
}
