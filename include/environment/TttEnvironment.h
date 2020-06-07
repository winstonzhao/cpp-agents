#pragma once

#include <array>
#include <vector>
#include <iostream>
#include <map>
#include <unordered_map>
#include <boost/functional/hash.hpp>

#include "environment/Environment.h"

namespace CppAgents::Environment::TttEnvironment
{
    constexpr int CROSS = 1;
    constexpr int CIRCLE = -1;
    constexpr int BLANK = 0;
    constexpr int DRAW = 0;

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
        using optimal_action_map_t = std::map<observation_t, std::pair<int, std::vector<action_t>>>;
        using state_action_map_t = std::unordered_map<observation_t, std::vector<action_t>, boost::hash<observation_t>>;

        static bool IsMax(observation_t obs)
        {
            return obs[LENGTH * LENGTH] == CROSS;
        }

        static state_action_map_t StateActionMap;
        static std::vector<action_t> GetActions(timestep_t ts)
        {
            auto action = StateActionMap.find(ts.observation);
            if (action != StateActionMap.end())
            {
                return (action->second);
            }

            std::vector<action_t> moves;
            for (int i = 0; i < LENGTH * LENGTH; i++)
            {
                if (ts.observation[i] == BLANK)
                {
                    moves.emplace_back(i);
                }
            }

            StateActionMap.insert({ts.observation, moves});

            return moves;
        }

        static std::vector<std::vector<int>> WinningGroups;
        static void InitializeWinningGroups()
        {
            if (WinningGroups.size() != 0)
            {
                return;
            }

            for (int i = 0; i < LENGTH; i++)
            {
                std::vector<int> row;
                std::vector<int> col;
                for (int j = 0; j < LENGTH; j++)
                {
                    row.push_back(i * LENGTH + j);
                    col.push_back(j * LENGTH + i);
                }
                WinningGroups.push_back(row);
                WinningGroups.push_back(col);
            }

            std::vector<int> lrDiag;
            std::vector<int> rlDiag;
            for (int i = 0; i < LENGTH; i++)
            {
                lrDiag.push_back((LENGTH + 1) * i);
                rlDiag.push_back(LENGTH - 1 + (LENGTH - 1) * i);
            }
            WinningGroups.push_back(rlDiag);
            WinningGroups.push_back(lrDiag);
        }

        // -1 implies we need to calculate this
        static std::pair<bool, int> IsGameOver(state_t &board, int numPieces = -1)
        {
            InitializeWinningGroups();

            if (numPieces == -1)
            {
                numPieces = 0;
                for (int piece : board)
                {
                    if (piece != BLANK)
                    {
                        numPieces++;
                    }
                }
            }

            if (numPieces < LENGTH * 2 - 1)
            {
                return {false, 0};
            }

            for (int symbol : {CROSS, CIRCLE})
            {
                for (const auto &group : WinningGroups)
                {
                    bool allSame = true;
                    for (auto place : group)
                    {
                        if (board[place] != symbol)
                        {
                            allSame = false;
                            break;
                        }
                    }
                    if (allSame)
                    {
                        return {true, symbol};
                    }
                }
            }

            if (numPieces == LENGTH * LENGTH)
            {
                return {true, DRAW};
            }

            return {false, 0};
        }

        struct node_t
        {
            node_t *parent;
            int action;
            reward_t reward;
            observation_t state;
            std::vector<node_t *> children;
        };

        static void MinimaxTreeSearch(optimal_action_map_t &map, node_t *node)
        {
            auto entry = map.find(node->state);
            if (entry != map.end())
            {
                node->reward = entry->second.first;
                return;
            }

            for (int i = 0; i < node->state.size(); i++)
            {
                if (node->state[i] == BLANK)
                {
                    observation_t newState = node->state;
                    newState[i] = node->state[LENGTH * LENGTH];
                    newState[LENGTH * LENGTH] *= -1;
                    auto goState = IsGameOver(newState);
                    if (!goState.first)
                    {
                        node->children.emplace_back(new node_t{node, i, -newState[LENGTH * LENGTH] * 1000, newState, {}});
                        MinimaxTreeSearch(map, node->children[node->children.size() - 1]);
                    }
                    else
                    {
                        node->children.emplace_back(new node_t{node, i, goState.second, newState, {}});
                    }
                }
            }

            for (node_t *child : node->children)
            {
                node->reward = node->state[LENGTH * LENGTH] == CROSS ? std::max(child->reward, node->reward) : std::min(child->reward, node->reward);
            }

            entry = map.insert({node->state, {node->reward, {}}}).first;

            for (node_t *child : node->children)
            {
                if (child->reward == node->reward)
                {
                    entry->second.second.emplace_back(child->action);
                }
            }
        }

        static void DestroyMinimaxTree(node_t *root)
        {
            for (node_t *child : root->children)
            {
                DestroyMinimaxTree(child);
            }

            delete root;
        }

        static optimal_action_map_t OptimalPolicy;
        static optimal_action_map_t GetStateToOptimalActionsMap()
        {
            optimal_action_map_t map;
            observation_t initialState;
            std::fill(initialState.begin(), initialState.end(), BLANK);
            initialState[LENGTH * LENGTH] = CROSS;

            node_t *root = new node_t{nullptr, 0, -1000, initialState, {}};
            MinimaxTreeSearch(map, root);
            DestroyMinimaxTree(root);
            return map;
        }

        template <typename PolicyType>
        static std::pair<int, int> IsPolicyOptimal(PolicyType policy)
        {
            if (OptimalPolicy.size() == 0)
            {
                OptimalPolicy = GetStateToOptimalActionsMap();
            }

            int totalStates = 0;
            int statesOptimal = 0;
            observation_t bad;
            std::vector<action_t> actions;
            action_t picked;
            for (const auto &stateActionsPair : GetStateToOptimalActionsMap())
            {
                totalStates++;
                auto &state = stateActionsPair.first;
                auto &optimalActions = stateActionsPair.second.second;
                // reward and step type don't matter for this, they are only used in training
                typename PolicyType::timestep_t ts{state, 0, Trajectory::MID};
                auto res = policy.Action(ts);

                if (std::find(optimalActions.begin(), optimalActions.end(), res.action) != optimalActions.end())
                {

                    statesOptimal++;
                }
                else
                {
                    bad = state;
                    actions = optimalActions;
                    picked = res.action;
                }
            }

            if (statesOptimal == totalStates - 1)
            {
                std::cout << "----------------------------" << std::endl;
                for (int i = 0; i < LENGTH; i++)
                {
                    for (int j = 0; j < LENGTH; j++)
                    {
                        std::cout << bad[i * LENGTH + j];
                    }
                    std::cout << std::endl;
                }
                std::cout << "Picked Action: " << picked << std::endl;
                std::cout << "Good actions: ";
                for (auto oa : actions)
                {
                    std::cout << oa << ", ";
                }
                std::cout << std::endl;
            }

            return {totalStates, statesOptimal};
        }

    public:
        TttEnvironment()
        {
            Reset();
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

            auto goState = IsGameOver(mBoard, mNumPieces);

            // someone has won (or drawn)
            if (goState.first)
            {
                auto oldBoard = mBoard;
                Reset();
                return {oldBoard, goState.second, Trajectory::LAST};
            }

            mLastTimeStep = {mBoard, goState.second, Trajectory::MID};
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

            if (IsGameOver(mBoard, mNumPieces).first)
            {
                Reset();
            }
        }

    private:
        state_t mBoard;
        timestep_t mLastTimeStep;
        int mNumPieces;
    };

    template <int LENGTH>
    std::vector<std::vector<int>> TttEnvironment<LENGTH>::WinningGroups{};

    template <int LENGTH>
    typename TttEnvironment<LENGTH>::optimal_action_map_t TttEnvironment<LENGTH>::OptimalPolicy{};

    template <int LENGTH>
    typename TttEnvironment<LENGTH>::state_action_map_t TttEnvironment<LENGTH>::StateActionMap{};
} // namespace CppAgents::Environment::TttEnvironment
