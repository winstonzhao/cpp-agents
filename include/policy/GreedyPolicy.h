#pragma once

#include "policy/Policy.h"
#include "trajectory/TimeStep.h"
#include "util/RandomProvider.h"
#include <map>
#include <functional>
#include <assert.h>
#include <algorithm>
#include <numeric>

namespace CppAgents::Policy
{
    template <
        typename ActionType,
        typename TimeStepType>
    class GreedyPolicy : Policy<
                             TimeStepType,
                             ActionType>
    {
    public:
        using parent_t = Policy<TimeStepType,
                                ActionType>;

        using timestep_t = typename parent_t::timestep_t;
        using action_t = typename parent_t::action_t;
        using policystep_t = typename parent_t::policystep_t;
        using get_distribution_t = std::function<std::vector<std::pair<double, ActionType>>(timestep_t)>;

        static bool dist_compare(std::pair<double, ActionType> a, std::pair<double, ActionType> b)
        {
            return a.first < b.first;
        }

    public:
        GreedyPolicy(get_distribution_t getDistribution) : GetDistribution{getDistribution}, GetRandom{GetRandomInt}
        {
        }

        policystep_t Action(timestep_t ts) override
        {
            const auto distribution = GetDistribution(ts);

            assert(distribution.size() != 0);

            const auto &max = std::max_element(distribution.begin(), distribution.end(), dist_compare);
            std::vector<action_t> bestActions;
            for (const auto &el : distribution)
            {
                if (el.first == max->first)
                {
                    bestActions.emplace_back(el.second);
                }
            }

            if (bestActions.size() == 1)
            {
                return {bestActions[0]};
            }

            auto random = GetRandom(0, bestActions.size() - 1);

            return {bestActions[random]};
        }

        void SetRandomProvider(get_random_int_t provider)
        {
            GetRandom = provider;
        }

    private:
        get_distribution_t GetDistribution;
        get_random_int_t GetRandom;
    }; // namespace CppAgents::Policy
} // namespace CppAgents::Policy