add_subdirectory(gtest)

include_directories(./gtest/googletest/include)

set(test_src
  run_all.cc
  ttt_environment_tests.cc
  greedy_policy_tests.cc
  random_policy_tests.cc
  epsilon_greedy_policy_tests.cc
  MMMC_agent_tests.cc
)

add_executable(test_agents ${test_src})

target_link_libraries(test_agents
  gtest
  ${AGENTS_LIB_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
  ${MATH_LIBS}
)

add_test(
  NAME tests
  COMMAND test_agents
)
