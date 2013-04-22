file(
  GLOB_RECURSE
  WSMS_SRC
  main.cpp
  MMU.cpp
  CPU.cpp
  Core.cpp
  Disassembler.cpp
  Debugger.cpp
  IndexInstructions.cpp
  VDP.cpp
  SDL/SDLRenderer.cpp
)

set(BIN_NAME ../wsms_bin)

find_package(SDL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/SDL2/include/)

find_library(SDL2_LIBRARY
  NAMES
  SDL2
  PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/SDL2/lib/
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/SDL2/lib64/)

message(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
  ${BIN_NAME}
  ${WSMS_SRC}
)

target_link_libraries(
  ${BIN_NAME}
  ${SDL2_LIBRARY}
 )
