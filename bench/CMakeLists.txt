set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)

add_subdirectory(benchmark)
include_directories(benchmark/include)

set(bench_cc
  bench_ttt.cc
)

add_executable(bench_ttt ${bench_cc})

target_link_libraries(bench_ttt
  benchmark
  ${AGENTS_LIB_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
  ${MATH_LIBS}
)
