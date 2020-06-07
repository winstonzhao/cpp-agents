set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)

add_subdirectory(benchmark)
include_directories(benchmark/include)

set(bench_cc
  bench_mmmc_ttt_gym.cc
)

add_executable(bench_mmmc_ttt_gym ${bench_cc})

target_link_libraries(bench_mmmc_ttt_gym
  benchmark
  ${AGENTS_LIB_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
  ${MATH_LIBS}
)
