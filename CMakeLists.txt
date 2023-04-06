cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)

# hunter module, keep up-to-date
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.24.14.tar.gz"
    SHA1 "00901c19eefc02d24b16705b5f5a2b4f093a73fb"
)

project(LABS)
option(BUILD_TESTS "Build tests" OFF)

hunter_add_package(GTest)
find_package(GTest CONFIG REQUIRED)

add_executable(
    solver_RK1
    ${CMAKE_CURRENT_SOURCE_DIR}/source/sadovskiy_alg_solver.cpp
)
add_executable(
    solver_LR2
    ${CMAKE_CURRENT_SOURCE_DIR}/source/convex_games_solver.cpp
)
add_executable(
    solver_LR1
    ${CMAKE_CURRENT_SOURCE_DIR}/source/solver.cpp
)

target_include_directories(
    solver_RK1
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/header
)
target_include_directories(
    solver_LR2
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/header
)
target_include_directories(
    solver_LR1
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/header
)

# testing
if(BUILD_TESTS)
    enable_testing()
    add_executable(
      quotient_test
      tests/quotient_test.cpp
    )
    target_link_libraries(
      quotient_test
      GTest::gtest_main
    )
    target_include_directories(
        quotient_test
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
    )
    add_executable(
      linal_test
      tests/lin_alg_ops_test.cpp
    )
    target_link_libraries(
      linal_test
      GTest::gtest_main
    )
    target_include_directories(
        linal_test
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
    )
    add_executable(
      matrix_method_test
      tests/matrix_method_test.cpp
    )
    target_link_libraries(
      matrix_method_test
      GTest::gtest_main
    )
    target_include_directories(
        matrix_method_test
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
    )
    add_executable(
      br_method_test
      tests/matrix_method_test.cpp
    )
    target_link_libraries(
      br_method_test
      GTest::gtest_main
    )
    target_include_directories(
        br_method_test
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
    )
    include(GoogleTest)
    gtest_discover_tests(quotient_test)
    gtest_discover_tests(linal_test)
    gtest_discover_tests(matrix_method_test)
    gtest_discover_tests(br_method_test)
endif()
