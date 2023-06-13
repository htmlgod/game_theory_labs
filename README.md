# Game Theory Solvers and Algorithms

## LR1 (LR2)

```
cmake -S. -B_build -DBUILD_TESTS=On
cmake --build _build --target solver_LR1
./_build/solver_LR1
```

## LR2 (LR3)

```
cmake -S. -B_build -DBUILD_TESTS=On
cmake --build _build --target solver_LR2
./_build/solver_LR2
```

## LR3 (LR5)

```
cmake -S. -B_build
cmake --build _build --target solver_LR3
./_build/solver_LR3

```
## LR4 (LR6)

```
cmake -S. -B_build
cmake --build _build --target solver_LR4
./_build/solver_LR4 && dot -Tpng -Gimagescale=true -Gdpi=1000 out.gv -o result1.png && open result1.png
```

## LR5 (LR8)

```
cmake -S. -B_build
cmake --build _build --target solver_LR5
./_build/solver_LR5
```

## LR6 (LR7)

```
cmake -S. -B_build
cmake --build _build --target solver_LR6
./_build/solver_LR6
```
## RK1 (RK1)

```
cmake -S. -B_build -DBUILD_TESTS=On
cmake --build _build --target solver_RK1
./_build/solver_RK1
```
## RK2 (RK2)

```
cd search_games
python3 -m venv env
source ./env/bin/activate
pip3 install -r requirements.txt
python3 search.py
```
