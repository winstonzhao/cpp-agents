#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include "agent/MMMCAgent.h"
#include "environment/TttEnvironment.h"
#include "trajectory/Trajectory.h"
#include "util/RandomProvider.h"

namespace CppAgents::MmmcTttGym
{
    template <int LENGTH>
    class MmmcTttGym
    {
    public:
        using env_t = Environment::TttEnvironment::TttEnvironment<LENGTH>;
        using ts_t = Trajectory::TimeStepType<typename env_t::observation_t, typename env_t::reward_t, int>;
        using traj_t = Trajectory::Trajectory<ts_t, int>;
        using agent_t = Agent::MMMCAgent<traj_t>;
        using collect_policy_t = typename agent_t::collect_policy_t;
        using policystep_t = typename collect_policy_t::policystep_t;

        struct IterationData
        {
            int timesteps;
            double optimality;
            double loss;
        };

    public:
        MmmcTttGym(double alpha, double epsilon) : mEnv{},
                                                   mAgent{env_t::GetActions, env_t::IsMax, alpha, epsilon},
                                                   mCollectPolicy{mAgent.GetCollectPolicy()},
                                                   mData{},
                                                   mTimesteps{0}
        {
        }

        double Test()
        {
            const auto res = env_t::IsPolicyOptimal(mAgent.GetPolicy());
            return (double)res.second / res.first;
        }

        double Train(int timesteps)
        {
            int tsCount = 0;
            double loss = 0;

            while (tsCount < timesteps)
            {
                ts_t next, prev;
                next.stepType = Trajectory::FIRST;
                policystep_t res;
                while (next.stepType != Trajectory::LAST)
                {
                    prev = mEnv.GetCurrentTimeStep();
                    res = mCollectPolicy.Action(prev);
                    next = mEnv.Step(res.action);
                    mData.emplace_back(traj_t{prev, res.action, next});
                    mTimesteps++;
                    tsCount++;
                }

                loss += mAgent.Train(mData);
                mData.clear();
            };

            return loss / timesteps;
        }

        IterationData TrainAndTest(int timesteps)
        {
            const auto loss = Train(timesteps);
            const auto optimalPercentage = Test();
            return {mTimesteps, optimalPercentage, loss};
        }

    private:
        env_t mEnv;
        agent_t mAgent;
        collect_policy_t mCollectPolicy;
        std::vector<traj_t> mData;
        int mTimesteps;
    };

} // namespace CppAgents::MmmcTttGym