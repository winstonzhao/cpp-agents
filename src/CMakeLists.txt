# Source files (except the main for the executable):
set(agents_src
  test.cc
)

# Compile the library
add_library(${AGENTS_LIB_NAME} ${AGENTS_LIB_TYPE} ${agents_src})

target_link_libraries(${AGENTS_LIB_NAME} ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIBS})
install(TARGETS ${AGENTS_LIB_NAME} LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)