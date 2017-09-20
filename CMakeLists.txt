cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
cmake_policy(SET CMP0054 OLD)
cmake_policy(SET CMP0045 OLD)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(ProjectDependency)

project(uev C)
project_dependency(uds GIT_REPOSITORY https://github.com/uael/uds.git)

set(uev_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(uev_TEST_DIR ${CMAKE_CURRENT_LIST_DIR}/test)

file(GLOB_RECURSE uev_HDRS ${uev_HDRS} ${uev_INC_DIR}/uev/*.h)
set(uev_HDR ${uev_INC_DIR}/uev.h)

add_library(uev INTERFACE)
target_include_directories(uev INTERFACE ${uev_INC_DIR})
target_link_libraries(uev INTERFACE uds)

get_directory_property(uev_PARENT PARENT_DIRECTORY)
if (NOT ${uev_PARENT})
  set(uev_DEVEL TRUE)
elseif (NOT ${uev_DEVEL})
  set(uev_DEVEL FALSE)
endif ()
if (${uev_DEVEL} AND EXISTS ${uev_TEST_DIR})
  project_dependency(cute GIT_REPOSITORY https://github.com/uael/cute.git)
  if (NOT uev_PARENT)
    enable_testing()
  endif ()

  file(GLOB uev_TEST_SRCS ${uev_TEST_SRCS} ${uev_TEST_DIR}/*.c)
  foreach (uev_TEST_SRC ${uev_TEST_SRCS})
    get_filename_component(uev_TEST_NAME ${uev_TEST_SRC} NAME_WE)
    add_executable(test_${uev_TEST_NAME} ${uev_TEST_SRC})
    add_dependencies(test_${uev_TEST_NAME} uev cute)
    target_link_libraries(test_${uev_TEST_NAME} uev cute)
    add_test(${uev_TEST_NAME} test_${uev_TEST_NAME})
  endforeach ()
endif ()

install(FILES ${uev_HDRS}
  DESTINATION include/uev)
install(FILES ${uev_HDR}
  DESTINATION include)