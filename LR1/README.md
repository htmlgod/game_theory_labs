# Лабораторная работа №2
Цель работы - Изучение аналитического (обратной матрицы) и численного (Брауна-Робинсона) методов нахождения смешанных стратегий в антагонистической игре двух лиц в нормальной форме.

## TODO COMMON

- [] Quotient class division
- [] File i/o
- [] CLI args 
- [] Usage statement

## TODO MATRIX METHOD

- [] Tests

## TODO BR METHOD

- [] Option to use random strat selection or user defined
- [] Option to use strat selection path (e.g array with indexes of strats to select from when we have multiply variants of strategies)
- [] Tests

## Build and Run
```
cmake -S. -B_build -DBUILD_TESTS=On
cmake --build _build
./_build/solve_by_matrix_method
./_build/solve_by_br_method
```
