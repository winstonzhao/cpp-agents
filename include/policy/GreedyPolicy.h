#pragma once

#include "policy/Policy.h"
#include "trajectory/TimeStep.h"
#include "util/RandomProvider.h"
#include <map>
#include <functional>

namespace CppAgents::Policy::GreedyPolicy
{
    template <
        typename ActionType,
        typename TimeStepType>
    class GreedyPolicy : Policy<
                             TimeStepType,
                             ActionType,
                             std::multimap<int, ActionType>>
    {
    public:
        using parent = Policy<TimeStepType,
                              ActionType,
                              std::multimap<int, ActionType>>;

        using timestep_t = typename parent::timestep_t;
        using action_t = typename parent::action_t;
        using info_t = typename parent::info_t;
        using policystep_t = typename parent::policystep_t;
        using get_distribution_t = std::function<info_t(timestep_t)>;

    public:
        GreedyPolicy(get_distribution_t getDistribution)
        {
            GetDistribution = getDistribution;
            GetRandom = GetRandomInt;
        }

        policystep_t Action(timestep_t ts) override
        {
            const auto &distribution = GetDistribution(ts);

            assert(distribution.size() != 0);

            if (distribution.size() > 1)
            {
                auto best = distribution.rbegin();
                int numBest = 1;
                for (auto i = ++distribution.rbegin(); i != distribution.rend(); i++)
                {
                    if (i->first == best->first)
                    {
                        numBest++;
                    }
                    else
                    {
                        break;
                    }
                }

                auto random = GetRandom(0, numBest - 1);

                for (random; random > 0; random--)
                {
                    best++;
                }

                return {best->second, distribution};
            }

            return {distribution.rbegin()->second, distribution};
        }

        void SetRandomProvider(get_random_int_t provider)
        {
            GetRandom = provider;
        }

    public:
        get_distribution_t GetDistribution;
        get_random_int_t GetRandom;
    };
} // namespace CppAgents::Policy::GreedyPolicy