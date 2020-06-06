#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <map>
#include "environment/Environment.h"

namespace CppAgents::Environment::TttEnvironment
{
    constexpr int CROSS = 1;
    constexpr int CIRCLE = -1;
    constexpr int BLANK = 0;

    template <int LENGTH>
    class TttEnvironment : Environment<std::array<int, LENGTH * LENGTH + 1>, int, int> // last index is turn
    {
    public:
        using parent_t = Environment<std::array<int, LENGTH * LENGTH + 1>, int, int>;

        using timestep_t = typename parent_t::timestep_t;
        using observation_t = typename parent_t::observation_t;
        using action_t = typename parent_t::action_t;
        using reward_t = typename parent_t::reward_t;
        using state_t = typename parent_t::state_t;
        using discount_t = typename parent_t::discount_t;
        using seed_t = typename parent_t::seed_t;

        static bool IsMax(timestep_t ts)
        {
            return ts.observation[LENGTH * LENGTH + 1] == CROSS;
        }

        static std::vector<action_t> GetActions(timestep_t ts)
        {
            std::vector<action_t> moves;
            for (int i = 0; i < LENGTH * LENGTH; i++)
            {
                if (ts.observation[i] == BLANK)
                {
                    moves.emplace_back(i);
                }
            }
            return moves;
        }

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
            mBoard[LENGTH * LENGTH] = CROSS;
            mNumPieces = 0;
            mLastTimeStep = {mBoard, 0, Trajectory::FIRST};
            return mLastTimeStep;
        }

        timestep_t Step(action_t action) override
        {
            assert(mBoard[action] == BLANK);

            int pieceToPlace = mBoard[LENGTH * LENGTH];
            mBoard[action] = pieceToPlace;
            mBoard[LENGTH * LENGTH] *= -1;
            mNumPieces++;

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
            for (int piece : state)
            {
                if (piece == CIRCLE || piece == CROSS)
                {
                    mNumPieces++;
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
            if (mNumPieces <= LENGTH * 2 - 1)
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
        state_t mBoard;
        timestep_t mLastTimeStep;
        int mNumPieces;
    };
} // namespace CppAgents::Environment::TttEnvironment