#pragma once

namespace CppAgents::Policy
{
    template <
        typename TimeStepType,
        typename ActionType,
        typename PolicyStateType,
        typename InfoType>
    class Policy
    {
    public:
        template <
            typename TActionType, typename TPolicyStateType, typename TInfoType>
        struct PolicyStepType
        {
            TActionType action;
            TPolicyStateType state;
            TInfoType info;
        };

        using timestep_t = TimeStepType;
        using action_t = ActionType;
        using policystate_t = PolicyStateType;
        using info_t = InfoType;
        using policystep_t = PolicyStepType<ActionType, PolicyStateType, InfoType>;

    public:
        virtual policystate_t GetInitialState();
        virtual policystep_t Action(TimeStepType ts, PolicyStateType ps);
    };
} // namespace CppAgents::Policy