cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)

project(solver_RK1)
add_executable(
    ${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/sadovskiy_alg_solver.cpp
)

target_include_directories(
    ${CMAKE_PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/LR1/header
)
