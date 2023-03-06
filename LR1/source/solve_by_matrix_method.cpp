#include <iostream>

#include <matrix_method.hpp>
#include <matrix_operations.hpp>
#include <quotient.hpp>

template<typename T>
void fill(std::vector<std::vector<T> >& vec, size_t size){
    vec.resize(size);
    for(int i = 0; i < size; i++){
        vec[i].resize(size);
        for(int j = 0; j < size; j++){
            std::cin >> vec[i][j];
        }
    }
}

auto main(int argc, char** argv) -> int {
    std::cout << "### Matrix game solver (inverse matrix method)" << '\n';
    std::cout << "Enter matrix size (only n): ";
    size_t size;
    std::cin >> size;
    std::cout << "Enter game cost delta (you can use fractionals, like " << std::quoted("1/2") << "): ";
    Q delta;
    std::cin >> delta;

    std::vector<std::vector<Q>> game_matrix;
    std::cout << "Enter game matrix: ";
    fill(game_matrix, size);
    std::cout << "Entered game matrix:" << std::endl;
    print_matrix(game_matrix);
    MatrixMethodSolver<Q> m_solver{game_matrix};
    m_solver.init();
    m_solver.solve();
    std::cout << "Player A strategies:" << std::endl;
    print_vector(m_solver.get_solution_playerA(), 4);
    std::cout << "Player B strategies:" << std::endl;
    print_vector(m_solver.get_solution_playerB(), 4);
    std::cout << "Game Cost:" << std::endl;
    std::cout << m_solver.get_gamecost(delta) << std::endl;

    std::cout << "Vector u:" << std::endl;
    print_vector(m_solver.get_u_vector(), 4);

    std::cout << "Common value:" << std::endl;
    std::cout << m_solver.get_common_value() << std::endl;
}
