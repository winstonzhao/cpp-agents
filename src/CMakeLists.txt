# Source files (except the main for the executable):
set(agents_src
  main.cc
)

# Compile the library
add_library(${AGENTS_LIB_NAME} ${AGENTS_LIB_TYPE} ${agents_src})
