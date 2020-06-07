#pragma once

#include "policy/Policy.h"
#include "trajectory/TimeStep.h"
#include "util/RandomProvider.h"
#include "policy/GreedyPolicy.h"
#include "policy/RandomPolicy.h"
#include <map>
#include <functional>
#include <assert.h>

namespace CppAgents::Policy
{
    template <
        typename ActionType,
        typename TimeStepType>
    class EpsilonGreedyPolicy : Policy<
                                    TimeStepType,
                                    ActionType,
                                    double>
    {
    public:
        using parent_t = Policy<TimeStepType,
                                ActionType,
                                double>;

        using timestep_t = typename parent_t::timestep_t;
        using action_t = typename parent_t::action_t;
        using info_t = typename parent_t::info_t;
        using policystep_t = typename parent_t::policystep_t;
        using get_distribution_t = std::function<std::vector<std::pair<double, ActionType>>(timestep_t)>;
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
            action_t action = (GetRandom(0, 1) > mEpsilon ? mGreedyPolicy.Action(ts) : mRandomPolicy.Action(ts)).action;
            DecayEpsilon();
            return {action, mEpsilon};
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

        void SetEpsilon(double epsilon)
        {
            mEpsilon = epsilon;
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