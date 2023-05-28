#include <convex_game_solver.hpp>

auto main() -> int {
    auto params = std::make_tuple(-3., 2.4, 6., -0.6, -4.8);
    ConvexGamesSolver solver{params};
    if (solver.solve_analytic()) {
        std::cout << "analytic solution" << std::endl;
        auto[gc, player_A_solution, player_B_solution] = solver.get_solution();
        std::cout << "Player A strategies: ";
        std::cout << player_A_solution << std::endl;
        std::cout << "Player B strategies: ";
        std::cout << player_B_solution << std::endl;
        std::cout << "Game Cost v = " << gc << std::endl;
        {
            solver.solve();
            auto[gc, player_A_solution, player_B_solution] = solver.get_solution();
            std::cout << "Player A strategy: ";
            std::cout << player_A_solution << std::endl;
            std::cout << "Player B strategy: ";
            std::cout << player_B_solution << std::endl;
            std::cout << "Game Cost v = " << gc << std::endl;
        }
    }
    return 0;
}
