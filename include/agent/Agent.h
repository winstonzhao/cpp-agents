#pragma once

#include "trajectory/Trajectory.h"
#include <vector>

namespace CppAgents::Agent
{
    template <
        typename TimeStepType,
        typename ActionType,
        typename PolicyType,
        typename CollectPolicyType,
        typename LossInfoType>
    class Agent
    {
    public:
        using timestep_t = TimeStepType;
        using action_t = ActionType;
        using policy_t = PolicyType;
        using collect_policy_t = CollectPolicyType;
        using loss_info_t = LossInfoType;

    public:
        // TODO: This should probably be a generic container
        virtual LossInfoType Train(std::vector<Trajectory::Trajectory<TimeStepType, ActionType>> data);
    };
} // namespace CppAgents::Agent