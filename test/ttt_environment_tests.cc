#include <iostream>
#include "gtest/gtest.h"
#include "environment/TttEnvironment.h"
#include "trajectory/TimeStep.h"

namespace CppAgents::Environment::TttEnvironment
{

  constexpr int LENGTH = 3;
  constexpr int ARRAY_SIZE = LENGTH * LENGTH + 1;

  TEST(TTT_ENVRIONMENT, CROSS_WIN_ROW)
  {
    TttEnvironment<LENGTH> ttt;
    std::array<int, ARRAY_SIZE> board = {
        CROSS, CROSS, BLANK,
        CIRCLE, CIRCLE, BLANK,
        CROSS, CIRCLE, BLANK,
        CROSS};
    ttt.SetState(board);
    const auto result = ttt.Step(2);
    EXPECT_EQ(result.reward, CROSS);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[2] = CROSS;
    boardCpy[ARRAY_SIZE - 1] = CIRCLE;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, CROSS_WIN_COL)
  {
    TttEnvironment<LENGTH> ttt;
    std::array<int, ARRAY_SIZE> board{
        CROSS, CIRCLE, BLANK,
        CROSS, CIRCLE, BLANK,
        BLANK, BLANK, BLANK,
        CROSS};
    ttt.SetState(board);
    const auto result = ttt.Step(6);
    EXPECT_EQ(result.reward, CROSS);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[6] = CROSS;
    boardCpy[ARRAY_SIZE - 1] = CIRCLE;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, CIRCLE_WIN_LRDIAG)
  {
    TttEnvironment<LENGTH> ttt;
    std::array<int, ARRAY_SIZE> board{
        CIRCLE, CROSS, CROSS,
        CROSS, CIRCLE, BLANK,
        BLANK, BLANK, BLANK,
        CIRCLE};
    ttt.SetState(board);
    const auto result = ttt.Step(8);
    EXPECT_EQ(result.reward, CIRCLE);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[8] = CIRCLE;
    boardCpy[ARRAY_SIZE - 1] = CROSS;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, CIRCLE_WIN_RLDIAG)
  {
    TttEnvironment<LENGTH> ttt;
    std::array<int, ARRAY_SIZE> board{
        CROSS, BLANK, BLANK,
        CROSS, CIRCLE, BLANK,
        CIRCLE, CROSS, BLANK,
        CIRCLE};
    ttt.SetState(board);
    const auto result = ttt.Step(2);
    EXPECT_EQ(result.reward, CIRCLE);
    EXPECT_EQ(result.stepType, Trajectory::LAST);

    auto boardCpy = board;
    boardCpy[2] = CIRCLE;
    boardCpy[ARRAY_SIZE - 1] = CROSS;
    EXPECT_EQ(result.observation, boardCpy);
  }

  TEST(TTT_ENVRIONMENT, RESET_AFTER_WIN)
  {
    TttEnvironment<LENGTH> ttt;
    std::array<int, ARRAY_SIZE> board{
        CROSS, BLANK, BLANK,
        CROSS, CIRCLE, BLANK,
        CIRCLE, CROSS, BLANK,
        CIRCLE};
    ttt.SetState(board);
    // Circle wins
    ttt.Step(2);
    // Should be blank
    auto res = ttt.GetCurrentTimeStep();
    std::array<int, ARRAY_SIZE> blankBoard{BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, CROSS};
    EXPECT_EQ(res.stepType, Trajectory::FIRST);
    EXPECT_EQ(res.reward, 0);
    EXPECT_EQ(res.observation, blankBoard);
  }

  TEST(TTT_ENVRIONMENT, GET_ACTIONS)
  {
    std::array<int, ARRAY_SIZE> board{
        CROSS, CIRCLE, BLANK,
        CROSS, CIRCLE, BLANK,
        BLANK, BLANK, BLANK,
        CROSS};
    auto res = TttEnvironment<LENGTH>::GetActions({board, 0, Trajectory::LAST});
    std::sort(res.begin(), res.end());
    std::vector<int> actions{2, 5, 6, 7, 8};
    EXPECT_EQ(res, actions);
  }

  TEST(TTT_ENVRIONMENT, OPTIMAL_ACTIONS)
  {
    auto actionMap = TttEnvironment<LENGTH>::GetStateToOptimalActionsMap();
    std::array<int, ARRAY_SIZE> board{
        BLANK, BLANK, BLANK,
        BLANK, CROSS, BLANK,
        BLANK, BLANK, BLANK,
        CIRCLE};

    const auto actions = actionMap.find(board)->second.second;
    std::vector<int> optimalActions{0, 2, 6, 8};
    EXPECT_EQ(actions, optimalActions);
  }

} // namespace CppAgents::Environment::TttEnvironment
