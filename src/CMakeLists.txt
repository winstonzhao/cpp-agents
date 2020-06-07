# Source files (except the main for the executable):
set(agents_src
  gyms/mmsl_ttt_gym.cc
)

# Compile the library
add_library(${AGENTS_LIB_NAME} ${AGENTS_LIB_TYPE} ${agents_src})

# add_executable(mmmc_ttt_gym gyms/mmmc_ttt_gym.cc)
add_executable(mmsl_ttt_gym gyms/mmsl_ttt_gym.cc)

target_link_libraries(${AGENTS_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBS})
install(TARGETS ${AGENTS_LIB_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)