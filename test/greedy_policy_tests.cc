#include <iostream>
#include "gtest/gtest.h"
#include "policy/GreedyPolicy.h"
#include "trajectory/TimeStep.h"
#include <array>

namespace CppAgents::Policy::GreedyPolicy
{

    TEST(GREEDY_POLICY, TESTEST)
    {
        GreedyPolicy<int, std::array<int, 9>> x;
        std::cout << "xd" << std::endl;
    }

} // namespace CppAgents::Policy::GreedyPolicy
