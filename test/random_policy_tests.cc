#include <iostream>
#include "gtest/gtest.h"
#include "policy/RandomPolicy.h"
#include "trajectory/TimeStep.h"

namespace CppAgents::Policy
{

    TEST(RANDOM_POLICY, PICKS_CORRECT_ACTION)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        RandomPolicy<std::string, ts_t> policy{[](ts_t) {
            auto actions = std::vector<std::string>{"action1", "action2", "action3"};
            return actions;
        }};

        policy.SetRandomProvider([](int, int) { return 0; });
        auto res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "action1");

        policy.SetRandomProvider([](int, int) { return 2; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "action3");

        policy.SetRandomProvider([](int, int) { return 1; });
        res = policy.Action({5, 5, Trajectory::FIRST});
        EXPECT_EQ(res.action, "action2");
    }

} // namespace CppAgents::Policy
