#pragma once

#include "policy/Policy.h"
#include "trajectory/TimeStep.h"
#include "util/RandomProvider.h"
#include <functional>

namespace CppAgents::Policy
{
    template <
        typename ActionType,
        typename TimeStepType>
    class RandomPolicy : Policy<
                             TimeStepType,
                             ActionType>
    {
    public:
        using parent_t = Policy<TimeStepType,
                                ActionType>;

        using timestep_t = typename parent_t::timestep_t;
        using action_t = typename parent_t::action_t;
        using policystep_t = typename parent_t::policystep_t;
        using get_actions_t = std::function<std::vector<action_t>(timestep_t)>;

    public:
        RandomPolicy(get_actions_t getActions) : GetActions{getActions}, GetRandom{GetRandomInt}
        {
        }

        policystep_t Action(timestep_t ts) override
        {
            const auto &actions = GetActions(ts);

            assert(actions.size() != 0);

            auto random = GetRandom(0, actions.size() - 1);
            auto begin = actions.begin();

            for (int i = 0; i < random; ++i)
            {
                begin++;
            }

            return {*begin};
        }

        void SetRandomProvider(get_random_int_t provider)
        {
            GetRandom = provider;
        }

    private:
        get_actions_t GetActions;
        get_random_int_t GetRandom;
    };
} // namespace CppAgents::Policy