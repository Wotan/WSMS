file(
  GLOB_RECURSE
  WSMS_SRC
  main.cpp
  MMU.cpp
  CPU.cpp
  Core.cpp
)

include_directories(../)

add_executable(
  ../wsms_bin
  ${WSMS_SRC}
)