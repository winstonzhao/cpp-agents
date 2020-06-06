#include <iostream>
#include "gtest/gtest.h"
#include "agent/MMMCAgent.h"
#include "trajectory/TimeStep.h"

namespace CppAgents::Agent
{

    TEST(MMMCAgent, PICKS_CORRECT_ACTION)
    {
        using ts_t = Trajectory::TimeStepType<int, int, int>;
        using traj_t = Trajectory::Trajectory<ts_t, int>;
        std::vector<traj_t> data;

        data.push_back({{0, 0, Trajectory::FIRST}, 1, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 2, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 3, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 4, {0, 1, Trajectory::MID}});
        data.push_back({{0, 1, Trajectory::MID}, 5, {0, 1, Trajectory::LAST}});

        MMMCAgent<traj_t> x;
        auto loss = x.Train(data);
        // all qValues are initialized to 0
        // 1 + 2 + 3 + 4 + 5 = 15
        EXPECT_EQ(loss, 15);
    }

} // namespace CppAgents::Agent
