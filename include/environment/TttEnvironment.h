#pragma once

#include <array>
#include <vector>
#include <iostream>
#include "environment/Environment.h"

namespace CppAgents::Environment::TttEnvironment
{
    // Main reason why this is hardcoded, is due to array usage,
    // I do wonder what the speed difference would be with std::vector,
    // I guess I can test that when the whole gym is complete for TTT
    constexpr int CROSS = 1;
    constexpr int CIRCLE = -1;
    constexpr int BLANK = 0;

    template <int LENGTH>
    class TttEnvironment : Environment<std::array<int, LENGTH * LENGTH>, int, int>
    {
    public:
        using parent = Environment<std::array<int, LENGTH * LENGTH>, int, int>;

        using timestep_t = typename parent::timestep_t;
        using observation_t = typename parent::observation_t;
        using action_t = typename parent::action_t;
        using reward_t = typename parent::reward_t;
        using state_t = typename parent::state_t;
        using discount_t = typename parent::discount_t;
        using seed_t = typename parent::seed_t;

    public:
        TttEnvironment()
        {
            Reset();

            for (int i = 0; i < LENGTH; i++)
            {
                std::vector<int> row;
                std::vector<int> col;
                for (int j = 0; j < LENGTH; j++)
                {
                    row.push_back(i * LENGTH + j);
                    col.push_back(j * LENGTH + i);
                }
                mWinningGroups.push_back(row);
                mWinningGroups.push_back(col);
            }

            std::vector<int> lrDiag;
            std::vector<int> rlDiag;
            for (int i = 0; i < LENGTH; i++)
            {
                lrDiag.push_back((LENGTH + 1) * i);
                rlDiag.push_back(LENGTH - 1 + (LENGTH - 1) * i);
            }
            mWinningGroups.push_back(rlDiag);
            mWinningGroups.push_back(lrDiag);
        }

        timestep_t GetCurrentTimeStep() override
        {
            return mLastTimeStep;
        }

        timestep_t Reset() override
        {
            std::fill(mBoard.begin(), mBoard.end(), BLANK);
            mNumCircle = 0;
            mNumCross = 0;
            mLastTimeStep = {mBoard, 0, Trajectory::FIRST};
            return mLastTimeStep;
        }

        timestep_t Step(action_t action) override
        {
            assert(mBoard[action] == BLANK);

            // Cross goes first
            int pieceToPlace;
            if (mNumCircle == mNumCross)
            {
                mNumCross++;
                pieceToPlace = CROSS;
            }
            else
            {
                mNumCircle++;
                pieceToPlace = CIRCLE;
            }
            mBoard[action] = pieceToPlace;

            auto goState = IsGameOver();

            // someone has won
            if (goState)
            {
                auto oldBoard = mBoard;
                Reset();
                return {oldBoard, goState, Trajectory::LAST};
            }

            mLastTimeStep = {mBoard, goState, Trajectory::MID};
            return mLastTimeStep;
        }

        observation_t GetInfo() override
        {
            return mBoard;
        }

        state_t GetState() override
        {
            return mBoard;
        }

        void SetState(state_t state) override
        {
            mBoard = state;
            mNumCircle = 0;
            mNumCross = 0;

            for (auto piece : mBoard)
            {
                if (piece == CROSS)
                {
                    mNumCross++;
                }
                else if (piece == CIRCLE)
                {
                    mNumCircle++;
                }
            }

            if (IsGameOver())
            {
                Reset();
            }
        }

    private:
        int IsGameOver()
        {
            if (mNumCircle < LENGTH && mNumCross < LENGTH)
            {
                return 0;
            }

            for (int symbol : {CROSS, CIRCLE})
            {
                for (const auto &group : mWinningGroups)
                {
                    bool allSame = true;
                    for (auto place : group)
                    {
                        if (mBoard[place] != symbol)
                        {
                            allSame = false;
                            break;
                        }
                    }
                    if (allSame)
                    {
                        return symbol;
                    }
                }
            }

            return 0;
        }

    private:
        std::vector<std::vector<int>> mWinningGroups;
        std::array<int, LENGTH * LENGTH> mBoard;
        timestep_t mLastTimeStep;
        int mNumCross = 0;
        int mNumCircle = 0;
    };
} // namespace CppAgents::Environment::TttEnvironment