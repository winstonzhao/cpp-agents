#pragma once

namespace CppAgents::Trajectory
{
    enum StepType
    {
        FIRST,
        MID,
        LAST
    };

    template <
        typename ObservationType,
        typename RewardType,
        typename DiscountType>
    struct TimeStepType
    {
        using observation_t = ObservationType;
        using reward_t = RewardType;
        using discount_t = DiscountType;

        ObservationType observation;
        RewardType reward;
        StepType stepType;
        DiscountType discount;
    };

} // namespace CppAgents::Trajectory