cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)

include("../cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
  SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)

project(ogonyok VERSION 0.1.0)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_library(${PROJECT_NAME} STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/csv.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/common/db.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/common/base.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/common/basic.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/common/individual.cpp
)

add_executable(demo
  ${CMAKE_CURRENT_SOURCE_DIR}/demo/main.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/common>"
  "$<INSTALL_INTERFACE:include>"
)

target_link_libraries(demo ${PROJECT_NAME})

if(BUILD_TESTS)
  add_executable(tests
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/run.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp
  )
  target_link_libraries(tests ${PROJECT_NAME} GTest::gtest_main)
  enable_testing()
  add_test(NAME unit_tests COMMAND tests)
endif()

install(TARGETS ${PROJECT_NAME} demo
  EXPORT "${PROJECT_NAME}-targets"
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(EXPORT "${PROJECT_NAME}-targets"
  NAMESPACE "${PROJECT_NAME}::"
  DESTINATION "lib/cmake"
)
