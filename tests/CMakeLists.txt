cmake_minimum_required(VERSION 2.6.4)

project(template)

include_directories(
  /usr/local/include/
  /home/freeman/software/include/
)

link_directories(
  /usr/local/lib/
  /usr/local/lib64/
  /home/freeman/software/lib/
)

FILE(GLOB_RECURSE LOCAL_CPP_FILES "${CMAKE_SOURCE_DIR}/../src/*.cpp")

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Wno-deprecated -Wno-invalid-offsetof -Wno-strict-aliasing -ansi -pthread -std=c++11 -DUT_TEST -Dprivate=public")

link_libraries(
  log4cplus
  gtest
  pthread
  z
  ssl
)

add_executable(test_close_hashmap
  ${LOCAL_CPP_FILES}
  ${CMAKE_SOURCE_DIR}/src/public/test_close_hashmap.cpp
)

add_executable(test_io_helper
  ${LOCAL_CPP_FILES}
  ${CMAKE_SOURCE_DIR}/src/public/test_io_helper.cpp
)
