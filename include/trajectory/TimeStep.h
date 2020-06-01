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
        ObservationType observation;
        RewardType reward;
        DiscountType discount;
        Trajectory::StepType stepType;
    };

} // namespace CppAgents::Trajectory