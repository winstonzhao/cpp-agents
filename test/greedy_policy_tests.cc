#include <iostream>
#include "gtest/gtest.h"
#include "policy/GreedyPolicy.h"
#include "trajectory/TimeStep.h"

namespace CppAgents::Policy
{

    TEST(GREEDY_POLICY, PICKS_HIGHEST_REWARD_SINGLE)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        GreedyPolicy<std::string, ts_t> policy{[](ts_t) {
            std::vector<std::pair<double, std::string>> actions;
            actions.emplace_back(std::make_pair(3, "best"));
            actions.emplace_back(std::make_pair(1, "worst"));
            actions.emplace_back(std::make_pair(2, "middle"));
            actions.emplace_back(std::make_pair(2, "middle2"));
            return actions;
        }};

        const auto res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "best");
    }

    TEST(GREEDY_POLICY, PICKS_HIGHEST_REWARD_MULTIPLE)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        GreedyPolicy<std::string, ts_t> policy{[](ts_t ts) {
            std::vector<std::pair<double, std::string>> actions;
            actions.emplace_back(std::make_pair(1, "worst"));
            actions.emplace_back(std::make_pair(2, "middle"));
            actions.emplace_back(std::make_pair(3, "first"));
            actions.emplace_back(std::make_pair(3, "second"));
            actions.emplace_back(std::make_pair(3, "third"));
            return actions;
        }};

        policy.SetRandomProvider([](int, int) { return 0; });
        auto res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "first");

        policy.SetRandomProvider([](int, int) { return 2; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "third");

        policy.SetRandomProvider([](int, int) { return 1; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "second");
    }

} // namespace CppAgents::Policy
