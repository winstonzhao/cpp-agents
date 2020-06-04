#include "environment/TttEnvironment.h"
#include "trajectory/TimeStep.h"
#include <algorithm>
#include <assert.h>

namespace CppAgents::Environment::TttEnvironment
{
    TttEnvironment::TttEnvironment()
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

    TttEnvironment::timestep_t TttEnvironment::GetCurrentTimeStep()
    {
        return mLastTimeStep;
    }

    TttEnvironment::timestep_t TttEnvironment::Reset()
    {
        std::fill(mBoard.begin(), mBoard.end(), BLANK);
        mNumCircle = 0;
        mNumCross = 0;
        mLastTimeStep = {mBoard, 0, Trajectory::FIRST};
        return mLastTimeStep;
    }

    TttEnvironment::timestep_t TttEnvironment::Step(action_t action)
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

    TttEnvironment::observation_t TttEnvironment::GetInfo()
    {
        return mBoard;
    }

    TttEnvironment::state_t TttEnvironment::GetState()
    {
        return mBoard;
    }

    void TttEnvironment::SetState(state_t state)
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

    int TttEnvironment::IsGameOver()
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
} // namespace CppAgents::Environment::TttEnvironment