#include <iostream>
#include <string>
#include <algorithm>

#include "gyms/mmmc_ttt_gym.h"
#include "agent/MMSLAgent.h"
#include "environment/TttEnvironment.h"
#include "trajectory/Trajectory.h"
#include "util/RandomProvider.h"

namespace CppAgents::MmslTttGym
{
  constexpr int LENGTH = 3;
  constexpr int ARRAY_SIZE = LENGTH * LENGTH + 1;

  using env_t = Environment::TttEnvironment::TttEnvironment<LENGTH>;
  using ts_t = Trajectory::TimeStepType<typename env_t::observation_t, typename env_t::reward_t, int>;
  using traj_t = Trajectory::Trajectory<ts_t, int>;
  using agent_t = Agent::MMSLAgent<traj_t>;

  static env_t env;
  static agent_t agent{env_t::GetActions, env_t::IsMax, 0.1, 0.2};
  static auto trainingPolicy = agent.GetCollectPolicy();

  std::vector<traj_t> data;

  void Train(int numSteps)
  {
    int steps = 0;
    ts_t next, prev;
    decltype(trainingPolicy)::policystep_t res;

    while (steps < numSteps)
    {
      prev = env.GetCurrentTimeStep();
      res = trainingPolicy.Action(prev);
      next = env.Step(res.action);
      agent.Train(traj_t{prev, res.action, next});
      steps++;
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

} // namespace CppAgents::MmslTttGym

int main()
{
  using namespace CppAgents::MmslTttGym;
  TrainAndTest(100, 10000000);
  return 0;
}
