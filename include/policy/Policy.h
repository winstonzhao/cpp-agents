#pragma once

namespace CppAgents::Policy
{
    template <
        typename TimeStepType,
        typename ActionType,
        typename InfoType>
    class Policy
    {
    public:
        template <
            typename TActionType, typename TInfoType>
        struct PolicyStepType
        {
            TActionType action;
            TInfoType info;
        };

        using timestep_t = TimeStepType;
        using action_t = ActionType;
        using info_t = InfoType;
        using policystep_t = PolicyStepType<ActionType, InfoType>;

    public:
        virtual policystep_t Action(TimeStepType ts);
    };
} // namespace CppAgents::Policy