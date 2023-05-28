#include <matrix_method.hpp>
#include <br_method.hpp>

auto main(int argc, char** argv) -> int {
    std::vector<std::vector<double>> game_matrix = {
        {8, 12, 10},
        {1, 6, 19},
        {17, 11, 11}
    };
    std::cout << "Game matrix:" << std::endl;
    print_matrix(game_matrix);

    MatrixMethodSolver<double> m_solver{game_matrix};
    m_solver.init();
    m_solver.solve();
    {
        std::cout << "Solution given by matrix method:" << std::endl;
        auto[gc, player_A_solution, player_B_solution] = m_solver.get_solution(0);
        std::cout << "Player A strategies:" << std::endl;
        print_vector(player_A_solution, 4);
        std::cout << "Player B strategies:" << std::endl;
        print_vector(player_B_solution, 4);
        std::cout << "Game Cost v = " << gc << std::endl;
    }
    constexpr double epsilon = 1./10;
    BRMethodSolver<double> br_solver{game_matrix, epsilon};
    br_solver.init();
    br_solver.solve();
    std::cout << std::endl;
    {
        std::cout << "Solution given by Brown-Robinson method:" << std::endl;
        auto[gc, player_A_solution, player_B_solution] = br_solver.get_solution();
        std::cout << "Player A strategies:" << std::endl;
        print_vector(player_A_solution, 4);
        std::cout << "Player B strategies:" << std::endl;
        print_vector(player_B_solution, 4);
        std::cout << "Game Cost v = " << gc << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Solutions check: " << std::endl;
    auto[gc_m, plA, plB] = m_solver.get_solution(0);
    auto[gc_br, pA, pB] = br_solver.get_solution();
    if (std::abs(gc_m - gc_br) < epsilon) {
        std::cout << "|" << gc_m << " - " << gc_br << "| < " << epsilon << std::endl;
    }

    return 0;
}
