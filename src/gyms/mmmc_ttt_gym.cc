#include <iostream>
#include <string>
#include <algorithm>

#include "gyms/mmmc_ttt_gym.h"
#include "agent/MMMCAgent.h"
#include "environment/TttEnvironment.h"
#include "trajectory/Trajectory.h"
#include "util/RandomProvider.h"

namespace CppAgents::MmmcTttGym
{
  constexpr int LENGTH = 4;
  constexpr int ARRAY_SIZE = LENGTH * LENGTH + 1;

  using env_t = Environment::TttEnvironment::TttEnvironment<LENGTH>;
  using ts_t = Trajectory::TimeStepType<typename env_t::observation_t, typename env_t::reward_t, int>;
  using traj_t = Trajectory::Trajectory<ts_t, int>;
  using agent_t = Agent::MMMCAgent<traj_t>;

  static env_t env;
  static agent_t agent{env_t::GetActions, env_t::IsMax, 0.5, 0.9};
  static auto trainingPolicy = agent.GetCollectPolicy();

  std::vector<traj_t> data;

  void Train(int numEpisodes)
  {
    int episodes = 0;
    double sumLoss = 0;

    while (episodes < numEpisodes)
    {
      ts_t next, prev;
      next.stepType = Trajectory::FIRST;
      decltype(trainingPolicy)::policystep_t res;
      while (next.stepType != Trajectory::LAST)
      {
        prev = env.GetCurrentTimeStep();
        res = trainingPolicy.Action(prev);
        next = env.Step(res.action);
        data.emplace_back(traj_t{prev, res.action, next});
      }

      auto loss = agent.Train(data);
      episodes++;
      data.clear();
    };
  }

  void TrainAndTest(int nIterations, int nTrain)
  {
    for (int i = 0; i < nIterations; i++)
    {
      Train(nTrain);
      const auto res = env_t::IsPolicyOptimal(agent.GetPolicy());
      const auto numVals = agent.GetQValues().size();
      printf("Iteration: %d, Optimal Percentage: %f, States Seen: %ld, States Exist: %d\n", i, (double)res.second / res.first, numVals, res.first);
      // trainingPolicy.SetEpsilon(((1 - (double)res.second / res.first)));
    }
  }

} // namespace CppAgents::MmmcTttGym

int main()
{
  using namespace CppAgents::MmmcTttGym;
  TrainAndTest(1, 1);
  return 0;
}
