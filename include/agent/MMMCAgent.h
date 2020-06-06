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
                          Policy::GreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
                          Policy::EpsilonGreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
                          double>
    {
    public:
        using parent_t = Agent<
            std::vector<TrajectoryType>, // single episode of training
            typename TrajectoryType::action_t,
            Policy::GreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
            Policy::EpsilonGreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
            double>;
        using trajectory_t = TrajectoryType;
        using timestep_t = typename trajectory_t::timestep_t;
        using loss_info_t = typename parent_t::loss_info_t;
        using trainingdata_t = typename parent_t::trainingdata_t;
        using action_t = typename parent_t::action_t;
        using observation_t = typename timestep_t::observation_t;
        using reward_t = typename timestep_t::reward_t;
        using collect_policy_t = typename parent_t::collect_policy_t;
        using policy_t = typename parent_t::policy_t;
        using get_actions_t = std::function<std::vector<action_t>(timestep_t)>;
        using get_distribution_t = std::function<std::multimap<double, action_t>(timestep_t)>;
        using is_max_state_t = std::function<bool(observation_t)>;
        using qvalsmap_t = std::map<std::pair<observation_t, action_t>, float>;

    public:
        MMMCAgent(get_actions_t getActions,
                  is_max_state_t isMaxState,
                  double alpha = 0.05,
                  double epsilon = 0.1,
                  double epsilonDecayRate = 0,
                  double epsilonTerminalValue = 0) : GetActions{getActions},
                                                     IsMaxState{isMaxState},
                                                     mAlpha{alpha},
                                                     mEpsilon{epsilon},
                                                     mEpsilonDecayRate{mEpsilonDecayRate},
                                                     mEpsilonTerminal{epsilonTerminalValue}
        {
        }

        loss_info_t Train(trainingdata_t data) override
        {
            assert(data.size() != 0);

            std::vector<reward_t> cumulativeRewards(data.size());
            std::transform(
                data.rbegin(), data.rend(), cumulativeRewards.rbegin(), [](const trajectory_t &next) { return next.after.reward; });
            std::partial_sum(cumulativeRewards.rbegin(), cumulativeRewards.rend(), cumulativeRewards.rbegin(), std::plus<reward_t>());

            auto rewardIt = cumulativeRewards.begin();
            qvalsmap_t occurenceMap;
            loss_info_t loss = 0;

            for (const auto &traj : data)
            {
                auto occurences = GetOrCreateQValEntry(occurenceMap, {traj.before.observation, traj.action});
                occurences->second++;
                auto qVal = GetOrCreateQValEntry(mQValues, {traj.before.observation, traj.action});
                loss_info_t iterationLoss = (*rewardIt) - qVal->second;
                rewardIt++;
                loss += iterationLoss;
                qVal->second += (mAlpha / (occurences->second)) * iterationLoss;
            }

            return loss;
        }

        collect_policy_t GetCollectPolicy() override
        {
            return Policy::EpsilonGreedyPolicy<action_t, timestep_t>(
                GetDistributionFunction(),
                GetActions, mEpsilon, mEpsilonDecayRate, mEpsilonTerminal);
        }

        policy_t GetPolicy() override
        {
            return Policy::GreedyPolicy<action_t, timestep_t>(GetDistributionFunction());
        }

        void SetEpsilon(double epsilon)
        {
            mEpsilon = epsilon;
        }

    private:
        get_distribution_t GetDistributionFunction()
        {
            return [&](timestep_t ts) {
                const auto actions = GetActions(ts);
                std::multimap<double, action_t> dist;
                double multiplier = IsMaxState(ts.observation) ? 1 : -1;

                for (const auto &action : actions)
                {
                    auto qVal = GetOrCreateQValEntry(mQValues, {ts.observation, action});
                    dist.insert({qVal->second * multiplier, action});
                }

                return dist;
            };
        }

        typename qvalsmap_t::iterator GetOrCreateQValEntry(qvalsmap_t &map, typename qvalsmap_t::key_type key)
        {
            auto qVal = map.find(key);
            if (qVal == map.end())
            {
                qVal = map.insert({key, 0}).first;
            }
            return qVal;
        }

    private:
        get_actions_t GetActions;
        is_max_state_t IsMaxState;

    private:
        qvalsmap_t mQValues;
        double mAlpha;
        double mEpsilon;
        double mEpsilonDecayRate;
        double mEpsilonTerminal;
    };
} // namespace CppAgents::Agent