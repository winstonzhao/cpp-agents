#pragma once

#include "trajectory/TimeStep.h"

namespace CppAgents::Environment
{
    template <
        typename ObservationType,
        typename ActionType,
        typename RewardType = int,
        typename StateType = ObservationType,
        typename DiscountType = int,
        typename SeedType = int>
    class Environment
    {
    public:
        using timestep_t = Trajectory::TimeStepType<ObservationType, RewardType, DiscountType>;
        using observation_t = ObservationType;
        using action_t = ActionType;
        using reward_t = RewardType;
        using state_t = StateType;
        using discount_t = DiscountType;
        using seed_t = SeedType;

    public:
        virtual timestep_t GetCurrentTimeStep() = 0;
        virtual timestep_t Reset() = 0;
        virtual timestep_t Step(action_t action) = 0;
        virtual void Close() {}
        virtual void Render() {}
        virtual void Seed(seed_t seed) {}
        virtual observation_t GetInfo() = 0;
        virtual state_t GetState() = 0;
        virtual void SetState(state_t state) = 0;
    };
}; // namespace CppAgents::Environment