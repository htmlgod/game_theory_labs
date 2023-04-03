#include <iostream>
#include <functional>
#include <optional>
#include <tuple>

#include <br_method.hpp>

using matrix = std::vector<std::vector<double>>;

void solve_analytic() {

}

auto get_saddle_point(const matrix& m) {
    std::vector<double> min_row;
    min_row.reserve(m.size());

    std::vector<double> max_col;
    max_col.reserve(m[0].size());

    for (const auto& row: m) {
        min_row.push_back(get_min_element(row));
    }
    auto t_m = get_transposed_matrix(m);
    for (const auto& col: t_m) {
        max_col.push_back(get_max_element(col));
    }
    auto maxmin = get_max_element(min_row);
    auto minmax = get_min_element(max_col);
    if (maxmin == minmax) {
        size_t i = get_max_element_index(min_row);
        size_t j = get_min_element_index(max_col);
        auto N = m.size() - 1;
        return std::make_tuple(static_cast<double>(i)/N, static_cast<double>(j)/N, minmax);
    }
    std::cout << "No saddle point" << '\n';
    constexpr double epsilon = 1./10;
    BRMethodSolver<double> br_solver{m, epsilon};
    br_solver.init();
    br_solver.solve(false);
    std::cout << "Solution given by Brown-Robinson method:" << std::endl;
    auto [gc, player_A_solution, player_B_solution] = br_solver.get_solution();
    std::cout << "Player A strategies:" << std::endl;
    print_vector(player_A_solution, 4);
    std::cout << "Player B strategies:" << std::endl;
    print_vector(player_B_solution, 4);
    std::cout << "Game Cost v = " << gc << std::endl;
    return std::make_tuple(0.,0.,0.);
}

matrix create_mesh(size_t N, std::function<double(double, double)> H) {
    matrix res;
    res.resize(N+1);
    for (auto& row : res) row.resize(N+1);

    for (size_t i = 0; i <= N; ++i) {
        for (size_t j =0; j <= N; ++j) {
            res[i][j] = H(static_cast<double>(i)/N, static_cast<double>(j)/N);
        }
    }
    return res;
}

auto main() -> int {
    //auto [a,b,c,d,e] = std::make_tuple(-3, 2.4, 6, -0.6, -4.8);
    auto [a,b,c,d,e] = std::make_tuple(-3, 1.5, 3.6, -0.36, -2.88);
    auto H = [&](double x, double y) -> double {
        return x*x*a + y*y*b + c*x*y + d*x + e*y;
    };

    auto mesh = create_mesh(2, H);
    print_matrix(mesh, 10);
    auto [x, y, Hv] = get_saddle_point(mesh);
    std::cout << x << " " << y << " " << Hv << '\n';

    return 0;
}
