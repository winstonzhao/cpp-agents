#pragma once

#include <array>
#include <vector>
#include <iostream>
#include "environment/Environment.h"

namespace CppAgents::Environment
{
    constexpr int LENGTH = 4;
    constexpr int CROSS = 1;
    constexpr int CIRCLE = -1;
    constexpr int BLANK = 0;

    class TttEnvironment : Environment<std::array<int, LENGTH * LENGTH>, int, int>
    {
    public:
        TttEnvironment();
        timestep_t GetCurrentTimeStep() override;
        timestep_t Reset() override;
        timestep_t Step(action_t action) override;
        observation_t GetInfo() override;
        state_t GetState() override;
        void SetState(state_t state) override;

    private:
        int IsGameOver();

    private:
        std::vector<std::vector<int>> mWinningGroups;
        std::array<int, LENGTH * LENGTH> mBoard;
        timestep_t mLastTimeStep;
        int mNumCross = 0;
        int mNumCircle = 0;
    };
} // namespace CppAgents::Environment