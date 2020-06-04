add_subdirectory(gtest)

include_directories(./gtest/googletest/include)

set(test_src
  run_all.cc
  ttt_env_tests.cc
  greedy_policy_tests.cc
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
