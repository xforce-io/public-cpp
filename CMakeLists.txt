cmake_minimum_required(VERSION 2.6.4)

project(public-cpp)

include_directories(
  /usr/include/
  )

link_directories(
)

set(PUBLIC_CPP_OUTPUT_PATH public-cpp/)
set(PUBLIC_CPP_LIB_OUTPUT_PATH  ${PUBLIC_CPP_OUTPUT_PATH}/lib/public-cpp)
set(PUBLIC_CPP_INC_OUTPUT_PATH  ${PUBLIC_CPP_OUTPUT_PATH}/include/public-cpp)

set(PUBLIC_CPP_SRC_DIR "${CMAKE_SOURCE_DIR}/src/")

FILE(GLOB_RECURSE PUBLIC_CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Wno-deprecated -Wno-invalid-offsetof -Wno-strict-aliasing -O2")

add_library(public-cpp
  ${PUBLIC_CPP_FILES})

target_link_libraries(public-cpp
  log4cplus
  pthread
  rt
  z
  #tcmalloc
  )

add_custom_command(TARGET public-cpp
  PRE_BUILD

  #make public-cpp
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_CPP_OUTPUT_PATH} 
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_CPP_LIB_OUTPUT_PATH} 
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_CPP_INC_OUTPUT_PATH} 
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_CPP_INC_OUTPUT_PATH}/jsontype
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_CPP_INC_OUTPUT_PATH}/mpsc_fixed_pipe
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_CPP_INC_OUTPUT_PATH}/pool_objs
  COMMAND ${CMAKE_COMMAND} -E make_directory ${PUBLIC_CPP_INC_OUTPUT_PATH}/thread_privacy

  COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_CPP_SRC_DIR}/io_helper.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_CPP_SRC_DIR}/jsontype/jsontype.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/jsontype/
  COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_CPP_SRC_DIR}/lock_policy.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_CPP_SRC_DIR}/mpsc_fixed_pipe/default_notifier.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/mpsc_fixed_pipe/
  COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_CPP_SRC_DIR}/mpsc_fixed_pipe/mpsc_fixed_pipe.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/mpsc_fixed_pipe/
  COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_CPP_SRC_DIR}/pool_objs/pool_objs.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/pool_objs/
  COMMAND ${CMAKE_COMMAND} -E copy ${PUBLIC_CPP_SRC_DIR}/thread_privacy/thread_privacy.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/thread_privacy/
)

add_custom_command(TARGET public-cpp
  POST_BUILD

  #make public-cpp
  COMMAND cp libpublic-cpp.a ${PUBLIC_CPP_LIB_OUTPUT_PATH}
)
