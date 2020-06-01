#pragma once

#include "trajectory/TimeStep.h"

namespace CppAgents::Environments
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
        using TimeStepType = Trajectory::TimeStepType<ObservationType, RewardType, DiscountType>;

    public:
        virtual TimeStepType GetCurrentTimeStep();
        virtual TimeStepType Reset();
        virtual TimeStepType Step(ActionType action);
        virtual void Close();
        virtual void Render();
        virtual void Seed(SeedType seed);
        virtual TimeStepType GetInfo();
        virtual StateType GetState();
        virtual void SetState(StateType state);
    };
}; // namespace CppAgents::Environments