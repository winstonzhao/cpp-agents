#pragma once

#include "policy/Policy.h"
#include "trajectory/TimeStep.h"
#include <map>
#include <functional>

namespace CppAgents::Policy::GreedyPolicy
{
    template <
        typename ActionType,
        typename InfoType>
    class GreedyPolicy : Policy<
                             Trajectory::TimeStepType<InfoType, int, int>,
                             ActionType,
                             std::map<std::pair<InfoType, ActionType>, int>,
                             InfoType>
    {
    public:
        using parent = Policy<Trajectory::TimeStepType<InfoType, int, int>,
                              ActionType,
                              std::map<std::pair<InfoType, ActionType>, int>,
                              InfoType>;

        using timestep_t = typename parent::timestep_t;
        using action_t = typename parent::action_t;
        using policystate_t = typename parent::policystate_t;
        using info_t = typename parent::info_t;
        using policystep_t = typename parent::policystep_t;

    public:
        GreedyPolicy() {}

        policystate_t GetInitialState() override
        {
            return mState;
        }

        policystep_t Action(timestep_t ts, policystate_t ps) override
        {
            const auto xd = mState.begin();
            return {xd->first.second, mState, xd->first.first};
        }

    private:
        std::map<std::pair<InfoType, ActionType>, int> mState;
    };
} // namespace CppAgents::Policy::GreedyPolicy