#pragma once

#include "trajectory/TimeStep.h"

namespace CppAgents::Environment
{
    template <
        typename ObservationType,
        typename ActionType,
        typename RewardType,
        typename StateType,
        typename DiscountType,
        typename SeedType>
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
        virtual timestep_t GetCurrentTimeStep();
        virtual timestep_t Reset();
        virtual timestep_t Step(ActionType action);
        virtual void Close();
        virtual void Render();
        virtual void Seed(SeedType seed);
        virtual timestep_t GetInfo();
        virtual StateType GetState();
        virtual void SetState(StateType state);
    };
}; // namespace CppAgents::Environment