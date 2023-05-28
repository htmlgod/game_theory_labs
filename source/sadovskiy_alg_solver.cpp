#include <sadovskiy_algorithm.hpp>

auto main() -> int {
    std::vector<std::vector<double>> A = {
            {8, 12, 10},
            {1, 6, 19},
            {17, 11, 11}
    };
    auto B = get_transposed_matrix(A);
    std::cout << "Player A" << '\n';
    {
        SadovskyIterMethodSolver solver(A);

        solver.solve();
        auto [gc, strat] = solver.get_solution();
        std::cout << "Solution: " << std::endl;
        std::cout << "Game cost: " << gc << std::endl;
        std::cout << "Player A strat: ";
        print_vector(strat);
    }
    std::cout << "Player B" << '\n';
    {
        SadovskyIterMethodSolver solver(B);

        solver.solve();
        auto [gc, strat] = solver.get_solution();
        std::cout << "Solution: " << std::endl;
        std::cout << "Game cost: " << -gc << std::endl;
        std::cout << "Player B strat: ";
        print_vector(strat);
    }
}
