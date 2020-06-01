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
        // TODO: This should probably be a generic container
        virtual LossInfoType Train(std::vector<Trajectory::Trajectory<TimeStepType, ActionType>> data);
    };
} // namespace CppAgents::Agent