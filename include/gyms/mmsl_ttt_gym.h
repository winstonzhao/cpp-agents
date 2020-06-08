#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include "agent/MMSLAgent.h"
#include "environment/TttEnvironment.h"
#include "trajectory/Trajectory.h"
#include "util/RandomProvider.h"

namespace CppAgents::MmslTttGym
{
    template <int LENGTH>
    class MmslTttGym
    {
        using env_t = Environment::TttEnvironment::TttEnvironment<LENGTH>;
        using ts_t = Trajectory::TimeStepType<typename env_t::observation_t, typename env_t::reward_t, int>;
        using traj_t = Trajectory::Trajectory<ts_t, int>;
        using agent_t = Agent::MMSLAgent<traj_t>;
        using collect_policy_t = typename agent_t::collect_policy_t;
        using policystep_t = typename collect_policy_t::policystep_t;

        struct IterationData
        {
            int timesteps;
            double optimality;
            double loss;
        };

    public:
        MmslTttGym(
            double alpha,
            double epsilon,
            double lambda,
            double gamma) : mEnv{},
                            mAgent{env_t::GetActions, env_t::IsMax, alpha, epsilon, lambda, gamma},
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
            int steps = 0;
            ts_t next, prev;
            policystep_t res;
            double loss = 0;

            while (steps < timesteps)
            {
                prev = mEnv.GetCurrentTimeStep();
                res = mCollectPolicy.Action(prev);
                next = mEnv.Step(res.action);
                loss += mAgent.Train(traj_t{prev, res.action, next});
                steps++;
                mTimesteps++;
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
        traj_t mData;
        int mTimesteps;
    };
} // namespace CppAgents::MmslTttGym