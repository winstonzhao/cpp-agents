#pragma once

#include "trajectory/Trajectory.h"
#include "agent/Agent.h"
#include "policy/GreedyPolicy.h"
#include "policy/EpsilonGreedyPolicy.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <boost/functional/hash.hpp>

namespace CppAgents::Agent
{

    template <typename TrajectoryType>
    class MMSLAgent : Agent<
                          TrajectoryType,
                          typename TrajectoryType::action_t,
                          Policy::GreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
                          Policy::EpsilonGreedyPolicy<typename TrajectoryType::action_t, typename TrajectoryType::timestep_t>,
                          double>
    {
    public:
        using parent_t = Agent<
            TrajectoryType,
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
        using get_distribution_t = std::function<std::vector<std::pair<double, action_t>>(timestep_t)>;
        using is_max_state_t = std::function<bool(observation_t)>;
        using qvalsmap_t = std::unordered_map<std::pair<observation_t, action_t>, float, boost::hash<std::pair<observation_t, action_t>>>;

    public:
        MMSLAgent(get_actions_t getActions,
                  is_max_state_t isMaxState,
                  double alpha = 0.05,
                  double epsilon = 0.1,
                  double epsilonDecayRate = 0,
                  double epsilonTerminalValue = 0) : GetActions{getActions},
                                                     IsMaxState{isMaxState},
                                                     mAlpha{alpha},
                                                     mEpsilon{epsilon},
                                                     mEpsilonDecayRate{mEpsilonDecayRate},
                                                     mEpsilonTerminal{epsilonTerminalValue},
                                                     mCollectPolicy{GetDistributionFunction(),
                                                                    GetActions, mEpsilon, mEpsilonDecayRate, mEpsilonTerminal},
                                                     mPolicy{GetDistributionFunction()}
        {
        }

        loss_info_t Train(trainingdata_t data) override
        {
            auto delta = data.after.reward + 0.5 * (data.after.stepType == Trajectory::LAST ? 0 : GetOrCreateQValEntry(mQValues, {data.after.observation, mCollectPolicy.Action(data.after).action})->second) - GetOrCreateQValEntry(mQValues, {data.before.observation, data.action})->second;
            GetOrCreateQValEntry(mEligibilities, {data.before.observation, data.action})->second++;
            for (auto &eligPair : mEligibilities)
            {
                GetOrCreateQValEntry(mQValues, eligPair.first)->second += mAlpha * delta * eligPair.second;
                eligPair.second *= 0.5 * 0.5;
            }

            if (data.after.stepType == Trajectory::LAST)
            {
                mEligibilities.clear();
            }

            return delta;
        }

        collect_policy_t &GetCollectPolicy() override
        {
            return mCollectPolicy;
        }

        policy_t GetPolicy() override
        {
            return mPolicy;
        }

        qvalsmap_t &GetQValues()
        {
            return mQValues;
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
                typename get_distribution_t::result_type dist;
                double multiplier = IsMaxState(ts.observation) ? 1 : -1;

                for (const auto action : actions)
                {
                    auto qVal = GetOrCreateQValEntry(mQValues, {ts.observation, action});
                    dist.emplace_back(std::make_pair(qVal->second * multiplier, action));
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
        qvalsmap_t mEligibilities;
        double mAlpha;
        double mEpsilon;
        double mEpsilonDecayRate;
        double mEpsilonTerminal;
        collect_policy_t mCollectPolicy;
        policy_t mPolicy;
    };
} // namespace CppAgents::Agent