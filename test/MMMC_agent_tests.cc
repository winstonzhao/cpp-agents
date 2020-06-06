#include <iostream>
#include "gtest/gtest.h"
#include "agent/MMMCAgent.h"
#include "trajectory/TimeStep.h"

namespace CppAgents::Agent
{

    TEST(MMMCAgent, CORRECT_LOSS_SINGLE_ITERATION)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        using traj_t = Trajectory::Trajectory<ts_t, int>;
        std::vector<traj_t> data;

        data.push_back({{0, 0, Trajectory::FIRST}, 1, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 2, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 3, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 4, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 5, {0, 1, Trajectory::LAST}});

        MMMCAgent<traj_t> agent{
            [](ts_t) { return std::vector<int>{}; },
            [](ts_t::observation_t) { return true; }};
        auto loss = agent.Train(data);
        // rewards for all 5 timesteps/actions are 1
        // this means the observed qValues are the arithmetic sequence 1..5
        // all qValues are initialized to 0, thus the loss should be equal to
        // the sum of the qValues
        // 1 + 2 + 3 + 4 + 5 = 15
        EXPECT_EQ(loss, 15);
    }

    TEST(MMMCAgent, PICKS_CORRECT_ACTION_AFTER_TRAINING)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        using traj_t = Trajectory::Trajectory<ts_t, int>;
        MMMCAgent<traj_t> agent{
            [](ts_t) { return std::vector<int>{1, 2}; },
            [](ts_t::observation_t) { return true; }};

        std::vector<traj_t> data;

        // state 0 -> action 1 -> (state 1, reward 0)
        data.push_back({{0, 0, Trajectory::FIRST}, 1, {1, -2, Trajectory::MID}});
        // state 1 -> action 1 -> (state 2, reward -1)
        data.push_back({{1, -2, Trajectory::MID}, 1, {2, 1, Trajectory::MID}});
        // state 2 -> action 1 -> (state 3, reward 5)
        data.push_back({{2, 1, Trajectory::MID}, 1, {3, 0, Trajectory::LAST}});
        agent.Train(data);

        data.clear();
        // state 0 -> action 2 -> (state 1, reward 0)
        data.push_back({{0, 0, Trajectory::FIRST}, 2, {1, 1, Trajectory::MID}});
        // state 1 -> action 2 -> (state 2, reward 1)
        data.push_back({{1, 1, Trajectory::MID}, 2, {2, -1, Trajectory::MID}});
        // state 2 -> action 2 -> (state 3, reward 0)
        data.push_back({{2, -1, Trajectory::MID}, 2, {3, -1, Trajectory::LAST}});
        agent.Train(data);

        auto policy = agent.GetPolicy();
        // qVal(state = 0, action = 1) = -1
        // qVal(state = 0, action = 2) = 0
        // we choose action 2
        auto res = policy.Action({0, 0, Trajectory::MID});
        EXPECT_EQ(res.action, 2);

        // qVal(state = 1, action = 1) = 1
        // qVal(state = 1, action = 2) = -1
        // we choose action 1
        res = policy.Action({1, 0, Trajectory::MID});
        EXPECT_EQ(res.action, 1);

        // qVal(state = 2, action = (1|2)) = 0
        // both actions are equal so action is selected randomly
        policy.SetRandomProvider([](int, int) { return 1; });
        res = policy.Action({2, 0, Trajectory::MID});
        EXPECT_EQ(res.action, 2);
        policy.SetRandomProvider([](int, int) { return 0; });
        res = policy.Action({2, 0, Trajectory::MID});
        EXPECT_EQ(res.action, 1);
    }

    TEST(MMMCAgent, PICKS_CORRECT_ACTION_MIN_MAX)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        using traj_t = Trajectory::Trajectory<ts_t, int>;
        MMMCAgent<traj_t> agent{
            [](ts_t) { return std::vector<int>{1, 2}; },
            // max on even states, min on odd states
            [](ts_t::observation_t state) { return state % 2 == 0; }};

        std::vector<traj_t> data;

        // state 0 -> action 1 -> (state 1, reward 0)
        data.push_back({{0, 0, Trajectory::FIRST}, 1, {1, 1, Trajectory::MID}});
        // state 1 -> action 1 -> (state 2, reward -1)
        data.push_back({{1, 1, Trajectory::MID}, 1, {2, 1, Trajectory::MID}});

        agent.Train(data);

        data.clear();
        // state 0 -> action 2 -> (state 1, reward 0)
        data.push_back({{0, 0, Trajectory::FIRST}, 2, {1, -1, Trajectory::MID}});
        // state 1 -> action 2 -> (state 2, reward 1)
        data.push_back({{1, -1, Trajectory::MID}, 2, {2, -1, Trajectory::MID}});

        agent.Train(data);

        auto policy = agent.GetPolicy();
        // qVal(state = 0, action = 1) = 2
        // qVal(state = 0, action = 2) = -2
        // we choose action 2 as we are maximizing
        auto res = policy.Action({0, 0, Trajectory::MID});
        EXPECT_EQ(res.action, 1);

        // qVal(state = 1, action = 1) = 1
        // qVal(state = 1, action = 2) = -1
        // we choose action 1 as we are minimizing
        res = policy.Action({1, 0, Trajectory::MID});
        EXPECT_EQ(res.action, 2);
    }

} // namespace CppAgents::Agent
