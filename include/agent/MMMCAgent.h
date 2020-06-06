#pragma once

#include "trajectory/Trajectory.h"
#include "agent/Agent.h"
#include "policy/GreedyPolicy.h"
#include "policy/EpsilonGreedyPolicy.h"

#include <vector>
#include <map>
#include <numeric>
#include <algorithm>

namespace CppAgents::Agent
{

    template <typename TrajectoryType>
    class MMMCAgent : Agent<
                          std::vector<TrajectoryType>, // single episode of training
                          typename TrajectoryType::action_t,
                          Policy::EpsilonGreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
                          Policy::GreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
                          double>
    {
    public:
        using parent = Agent<
            std::vector<TrajectoryType>, // single episode of training
            typename TrajectoryType::action_t,
            Policy::EpsilonGreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
            Policy::GreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
            double>;
        using trajectory_t = TrajectoryType;
        using timestep_t = typename trajectory_t::timestep_t;
        using loss_info_t = typename parent::loss_info_t;
        using trainingdata_t = typename parent::trainingdata_t;
        using action_t = typename parent::action_t;
        using observation_t = typename timestep_t::observation_t;
        using reward_t = typename timestep_t::reward_t;

    public:
        loss_info_t Train(trainingdata_t data) override
        {
            assert(data.size() != 0);

            std::vector<reward_t> cumulativeRewards(data.size());
            std::transform(
                data.rbegin(), data.rend(), cumulativeRewards.rbegin(), [](const trajectory_t &next) { return next.after.reward; });
            std::partial_sum(cumulativeRewards.rbegin(), cumulativeRewards.rend(), cumulativeRewards.rbegin(), std::plus<reward_t>());

            auto rewardIt = cumulativeRewards.begin();
            loss_info_t loss = 0;

            for (const auto &traj : data)
            {
                auto qVal = mQValues.find({traj.before.observation, traj.action});
                if (qVal == mQValues.end())
                {
                    qVal = mQValues.insert({{traj.before.observation, traj.action}, 0}).first;
                }
                loss_info_t iterationLoss = (*rewardIt) - qVal->second;
                rewardIt++;
                loss += iterationLoss;
                qVal->second += 0.5 * iterationLoss;
            }

            return loss;
        }

    private:
        std::map<std::pair<observation_t, action_t>, float> mQValues;
    };
} // namespace CppAgents::Agent