#pragma once

#include "policy/Policy.h"
#include "trajectory/TimeStep.h"
#include "util/RandomProvider.h"
#include "policy/GreedyPolicy.h"
#include "policy/RandomPolicy.h"
#include <map>
#include <functional>

namespace CppAgents::Policy
{
    template <
        typename ActionType,
        typename TimeStepType>
    class EpsilonGreedyPolicy : Policy<
                                    TimeStepType,
                                    ActionType,
                                    bool>
    {
    public:
        using parent = Policy<TimeStepType,
                              ActionType,
                              bool>;

        using timestep_t = typename parent::timestep_t;
        using action_t = typename parent::action_t;
        using info_t = typename parent::info_t;
        using policystep_t = typename parent::policystep_t;
        using get_distribution_t = std::function<std::multimap<double, ActionType>(timestep_t)>;
        using get_actions_t = std::function<std::vector<ActionType>(timestep_t)>;

    public:
        EpsilonGreedyPolicy(
            get_distribution_t getDistribution,
            get_actions_t getActions,
            double epsilon,
            double epsilonDecayRate = 0,
            double epsilonTerminalValue = 0)
            : GetRandom{GetRandomFloat},
              mEpsilon{epsilon},
              mDecayRate{epsilonDecayRate},
              mTerminalValue{epsilonTerminalValue},
              mGreedyPolicy{getDistribution},
              mRandomPolicy{getActions}
        {
            assert(epsilon <= 1 && epsilon >= 0);
            assert(epsilonDecayRate <= 1 && epsilonDecayRate >= 0);
            assert(epsilonTerminalValue <= 1 && epsilonTerminalValue >= 0);
        }

        policystep_t Action(timestep_t ts) override
        {
            const auto random = GetRandom(0, 1);
            policystep_t ps;
            if (random > mEpsilon)
            {
                const auto &res = mGreedyPolicy.Action(ts);
                ps = {res.action, false};
            }
            else
            {
                const auto &res = mRandomPolicy.Action(ts);
                ps = {res.action, true};
            }

            DecayEpsilon();
            return ps;
        }

        void SetFloatRandomProvider(get_random_float_t provider)
        {
            GetRandom = provider;
        }

        void SetIntRandomProvider(get_random_int_t provider)
        {
            mGreedyPolicy.SetRandomProvider(provider);
            mRandomPolicy.SetRandomProvider(provider);
        }

    private:
        void DecayEpsilon()
        {
            if (mEpsilon > mTerminalValue)
            {
                mEpsilon = std::max(mEpsilon * (1 - mDecayRate), mTerminalValue);
            }
        }

    private:
        get_random_float_t GetRandom;

        double mEpsilon;
        double mDecayRate;
        double mTerminalValue;
        GreedyPolicy<action_t, timestep_t> mGreedyPolicy;
        RandomPolicy<action_t, timestep_t> mRandomPolicy;
    };
} // namespace CppAgents::Policy