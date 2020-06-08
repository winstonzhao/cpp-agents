set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)

add_subdirectory(benchmark)
include_directories(benchmark/include)

set(bench_cc
  bench_ttt.cc
  bench_main.cc
)

add_executable(bench_main ${bench_cc})

target_link_libraries(bench_main
  benchmark
  ${AGENTS_LIB_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
  ${MATH_LIBS}
)
