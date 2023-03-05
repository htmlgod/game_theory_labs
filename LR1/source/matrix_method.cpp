#include <quotient.hpp>
#include <matrix_operations.hpp>
#include <iostream>

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

    std::vector<std::vector<Q>> game_matrix;
    std::cout << "Enter game matrix: ";
    fill(game_matrix, size);

    std::cout << "Entered game matrix:" << std::endl;
    print_matrix(game_matrix);
    // init
    std::vector<Q> u(size, Q{1});
    std::cout << "Vector u:" << std::endl;
    print_vector(u);
    // common value computation (u*C^-1*u)
    std::vector<Q> buf{size};
    auto inversed = get_inversed_matrix(game_matrix);
    std::cout << "Inversed game matrix:" << std::endl;
    print_matrix(inversed);
    buf = vector_matrix_mul(u, inversed);
    std::cout << "Common value:" << std::endl;
    Q common_value = vector_vector_mul(buf, u);
    std::cout << common_value << std::endl;
    // A player strategies
    std::vector<Q> x_strat{size};
    x_strat = vector_matrix_mul(u,inversed);
    std::cout << "Player A strategies:" << std::endl;
    x_strat = vector_number_div(common_value, x_strat);
    print_vector(x_strat);
    // B player strategies
    std::cout << "Player B strategies:" << std::endl;
    std::vector<Q> y_strat{size};
    y_strat = matrix_vector_mul(inversed, u);
    y_strat = vector_number_div(common_value, y_strat);
    print_vector(y_strat);
}
