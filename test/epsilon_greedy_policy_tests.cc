#include <iostream>
#include "gtest/gtest.h"
#include "policy/EpsilonGreedyPolicy.h"
#include "trajectory/TimeStep.h"

namespace CppAgents::Policy::EpsilonGreedyPolicy
{

    TEST(EPSILON_GREEDY_POLICY, PICKS_CORRECT_ACTION_NO_DECAY)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        EpsilonGreedyPolicy<std::string, ts_t> policy{
            [](ts_t) {
                std::multimap<int, std::string> actions;
                actions.insert({3, "g_best"});
                actions.insert({3, "g_best2"});
                actions.insert({1, "g_worst"});
                actions.insert({2, "g_middle"});
                return actions;
            },
            [](ts_t) {
                auto actions = std::vector<std::string>{"r_1", "r_2", "r_3"};
                return actions;
            },
            0.1};

        policy.SetFloatRandomProvider([](float, float) { return 0.11; });
        policy.SetIntRandomProvider([](int, int) { return 1; });
        auto res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "g_best");

        policy.SetIntRandomProvider([](int, int) { return 0; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "g_best2");

        policy.SetFloatRandomProvider([](float, float) { return 0.09; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "r_1");

        policy.SetIntRandomProvider([](int, int) { return 2; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "r_3");

        policy.SetIntRandomProvider([](int, int) { return 1; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "r_2");
    }

    TEST(EPSILON_GREEDY_POLICY, PICKS_CORRECT_ACTION_DECAY)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        EpsilonGreedyPolicy<std::string, ts_t> policy{
            [](ts_t) {
                std::multimap<int, std::string> actions;
                actions.insert({3, "g_best"});
                actions.insert({3, "g_best2"});
                actions.insert({1, "g_worst"});
                actions.insert({2, "g_middle"});
                return actions;
            },
            [](ts_t) {
                auto actions = std::vector<std::string>{"r_1", "r_2", "r_3"};
                return actions;
            },
            0.1,
            0.1};

        // should choose random
        policy.SetFloatRandomProvider([](float, float) { return 0.095; });
        policy.SetIntRandomProvider([](int, int) { return 1; });
        auto res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "r_2");

        // due to epsilon decay, greedy policy should be used
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "g_best");
    }

    TEST(EPSILON_GREEDY_POLICY, REACHES_TERMINAL_VALUE)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        EpsilonGreedyPolicy<std::string, ts_t> policy{
            [](ts_t) {
                std::multimap<int, std::string> actions;
                actions.insert({3, "g_best"});
                actions.insert({3, "g_best2"});
                actions.insert({1, "g_worst"});
                actions.insert({2, "g_middle"});
                return actions;
            },
            [](ts_t) {
                auto actions = std::vector<std::string>{"r_1", "r_2", "r_3"};
                return actions;
            },
            0.1,
            0.5,
            0.04};

        // should choose random
        policy.SetFloatRandomProvider([](float, float) { return 0.095; });
        policy.SetIntRandomProvider([](int, int) { return 1; });
        auto res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "r_2");

        // due to epsilon decay, greedy policy should be used
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "g_best");

        // epsilon should be terminal at 0.04, if no terminal was set, epsilon would be at 0.025
        policy.SetFloatRandomProvider([](float, float) { return 0.041; }); // greedy is used
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "g_best");
        policy.SetFloatRandomProvider([](float, float) { return 0.03; }); // random is used
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "r_2");
    }

} // namespace CppAgents::Policy::EpsilonGreedyPolicy
