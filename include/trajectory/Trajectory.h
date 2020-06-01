#pragma once

namespace CppAgents::Trajectory
{
    template <
        typename TimeStepType,
        typename ActionType>
    class Trajectory
    {
    public:
        using timestep_t = TimeStepType;
        using action_t = ActionType;

    public:
        TimeStepType before;
        ActionType action;
        TimeStepType after;
    };
} // namespace CppAgents::Trajectory