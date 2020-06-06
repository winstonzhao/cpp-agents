#pragma once

#include <vector>

namespace CppAgents::Agent
{
    template <
        typename TrainingDataType,
        typename ActionType,
        typename PolicyType,
        typename CollectPolicyType,
        typename LossInfoType>
    class Agent
    {
    public:
        using trainingdata_t = TrainingDataType;
        using action_t = ActionType;
        using policy_t = PolicyType;
        using collect_policy_t = CollectPolicyType;
        using loss_info_t = LossInfoType;

    public:
        // TODO: This should probably be a generic container
        virtual loss_info_t Train(trainingdata_t data);
    };
} // namespace CppAgents::Agent