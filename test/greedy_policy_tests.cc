#include <iostream>
#include "gtest/gtest.h"
#include "policy/GreedyPolicy.h"
#include "trajectory/TimeStep.h"
#include <array>

namespace CppAgents::Policy::GreedyPolicy
{

    TEST(GREEDY_POLICY, PICKS_HIGHEST_REWARD_SINGLE)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        GreedyPolicy<std::string, ts_t> x{[](ts_t) {
            std::multimap<int, std::string> actions;
            actions.insert({3, "best"});
            actions.insert({1, "worst"});
            actions.insert({2, "middle"});
            actions.insert({2, "middle"});
            return actions;
        }};

        const auto res = x.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "best");
    }

    TEST(GREEDY_POLICY, PICKS_HIGHEST_REWARD_MULTIPLE)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        GreedyPolicy<std::string, ts_t> x{[](ts_t ts) {
            std::multimap<int, std::string> actions;
            actions.insert({1, "worst"});
            actions.insert({2, "middle"});
            actions.insert({3, "first"});
            actions.insert({3, "second"});
            actions.insert({3, "third"});
            return actions;
        }};

        x.SetRandomProvider([](int, int) { return 0; });
        auto res = x.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "third");

        x.SetRandomProvider([](int, int) { return 2; });
        res = x.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "first");

        x.SetRandomProvider([](int, int) { return 1; });
        res = x.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "second");
    }

} // namespace CppAgents::Policy::GreedyPolicy
