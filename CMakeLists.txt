cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)

project(LABS)
add_executable(
    solver_RK1
    ${CMAKE_CURRENT_SOURCE_DIR}/sadovskiy_alg_solver.cpp
)
add_executable(
    solver_LR3
    ${CMAKE_CURRENT_SOURCE_DIR}/convex_games_solver.cpp
)

target_include_directories(
    solver_RK1
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/LR1/header
)
target_include_directories(
    solver_LR3
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/LR1/header
)
