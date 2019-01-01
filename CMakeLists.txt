cmake_minimum_required(VERSION 2.6.4)

project(public-cpp)

include_directories(
  /usr/include/
  /home/freeman/software/include/
  )

link_directories(
  /home/freeman/software/lib/
)

set(PUBLIC_CPP_OUTPUT_PATH public-cpp/)
set(PUBLIC_CPP_LIB_OUTPUT_PATH  ${PUBLIC_CPP_OUTPUT_PATH}/lib/public-cpp)
set(PUBLIC_CPP_INC_OUTPUT_PATH  ${PUBLIC_CPP_OUTPUT_PATH}/include/public-cpp)

set(PUBLIC_CPP_SRC_DIR "${CMAKE_SOURCE_DIR}/src/")

FILE(GLOB_RECURSE PUBLIC_CPP_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Wno-deprecated -Wno-invalid-offsetof -Wno-strict-aliasing -std=c++11 -O2")

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
  COMMAND mkdir -p ${PUBLIC_CPP_LIB_OUTPUT_PATH}
  COMMAND mkdir -p ${PUBLIC_CPP_INC_OUTPUT_PATH}
  COMMAND mkdir -p ${PUBLIC_CPP_INC_OUTPUT_PATH}/jsontype
  COMMAND mkdir -p ${PUBLIC_CPP_INC_OUTPUT_PATH}/mpsc_fixed_pipe
  COMMAND mkdir -p ${PUBLIC_CPP_INC_OUTPUT_PATH}/pool_objs
  COMMAND mkdir -p ${PUBLIC_CPP_INC_OUTPUT_PATH}/thread_privacy
  COMMAND mkdir -p ${PUBLIC_CPP_INC_OUTPUT_PATH}/time

  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/buffer.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/close_hashmap.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/common.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/delay_del_ptr.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/events_driver.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/gmonitor.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/heap.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/io_helper.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/jsontype/jsontype.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/jsontype/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/lock_policy.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/mem_profile.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/mpsc_fixed_pipe/default_notifier.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/mpsc_fixed_pipe/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/mpsc_fixed_pipe/mpsc_fixed_pipe.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/mpsc_fixed_pipe/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/mp_tools.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/pool_objs/pool_objs.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/pool_objs/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/public.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/slice.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/thread_privacy/thread_privacy.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/thread_privacy/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/thread_private_pool.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/time/time.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/time/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/str_helper.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/multi_array_helper.hpp ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/simple_trie.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND cp ${PUBLIC_CPP_SRC_DIR}/req_tracer.h ${PUBLIC_CPP_INC_OUTPUT_PATH}/
  COMMAND git log | head -n 1 > ${PUBLIC_CPP_OUTPUT_PATH}/version
)

add_custom_command(TARGET public-cpp
  POST_BUILD

  #make public-cpp
  COMMAND cp libpublic-cpp.a ${PUBLIC_CPP_LIB_OUTPUT_PATH}
)
