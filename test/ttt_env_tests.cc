#include <iostream>
#include "gtest/gtest.h"
#include "environment/TttEnvironment.h"
#include "trajectory/TimeStep.h"

namespace CppAgents::Environment::TttEnvironment
{

  TEST(TTT_ENVRIONMENT, CROSS_WIN_ROW)
  {
    TttEnvironment ttt;
    std::array<int, LENGTH *LENGTH> board = {
        CROSS, CROSS, BLANK,
        CIRCLE, CIRCLE, BLANK,
        CROSS, CIRCLE, BLANK};
    ttt.SetState(board);
    const auto result = ttt.Step(2);
    EXPECT_EQ(result.reward, CROSS);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[2] = CROSS;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, CROSS_WIN_COL)
  {
    TttEnvironment ttt;
    std::array<int, LENGTH * LENGTH> board{
        CROSS, CIRCLE, BLANK,
        CROSS, CIRCLE, BLANK,
        BLANK, BLANK, BLANK};
    ttt.SetState(board);
    const auto result = ttt.Step(6);
    EXPECT_EQ(result.reward, CROSS);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[6] = CROSS;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, CIRCLE_WIN_LRDIAG)
  {
    TttEnvironment ttt;
    std::array<int, LENGTH * LENGTH> board{
        CIRCLE, CROSS, CROSS,
        CROSS, CIRCLE, BLANK,
        BLANK, BLANK, BLANK};
    ttt.SetState(board);
    const auto result = ttt.Step(8);
    EXPECT_EQ(result.reward, CIRCLE);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[8] = CIRCLE;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, CIRCLE_WIN_RLDIAG)
  {
    TttEnvironment ttt;
    std::array<int, LENGTH * LENGTH> board{
        CROSS, BLANK, BLANK,
        CROSS, CIRCLE, BLANK,
        CIRCLE, CROSS, BLANK};
    ttt.SetState(board);
    const auto result = ttt.Step(2);
    EXPECT_EQ(result.reward, CIRCLE);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[2] = CIRCLE;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, RESET_AFTER_WIN)
  {
    TttEnvironment ttt;
    std::array<int, LENGTH * LENGTH> board{
        CROSS, BLANK, BLANK,
        CROSS, CIRCLE, BLANK,
        CIRCLE, CROSS, BLANK};
    ttt.SetState(board);
    // Circle wins
    ttt.Step(2);
    // Should be blank
    auto res = ttt.GetCurrentTimeStep();
    std::array<int, LENGTH * LENGTH> blankBoard{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK};
    EXPECT_EQ(res.stepType, Trajectory::FIRST);
    EXPECT_EQ(res.reward, 0);
    EXPECT_EQ(res.observation, blankBoard);
  }

} // namespace CppAgents::Environment::TttEnvironment
