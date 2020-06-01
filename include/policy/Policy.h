#pragma once

namespace CppAgents::Policy
{
    template <
        typename TimeStepType,
        typename ActionType,
        typename PolicyStateType,
        typename InfoType,
        typename ObservationAndActionConstraintSplitter>
    class Policy
    {
    public:
        struct PolicyStepType
        {
            ActionType action;
            PolicyStateType state;
            InfoType info;
        };

    public:
        virtual PolicyStateType GetInitialState();
        virtual PolicyStepType Action(TimeStepType ts, PolicyStateType ps);
    };
} // namespace CppAgents::Policy