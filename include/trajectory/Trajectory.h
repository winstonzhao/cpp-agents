#pragma once

namespace CppAgents::Trajectory
{
    template <
        typename TimeStepType,
        typename ActionType>
    class Trajectory
    {
        TimeStepType before;
        ActionType action;
        TimeStepType after;
    };
} // namespace CppAgents::Trajectory