#include <functional>
#include <tuple>
#include <deque>

#include <br_method.hpp>

template <typename T>
class ConvexGamesSolver {
public:
    using matrix = std::vector<std::vector<T>>;
    ConvexGamesSolver(T a, T b, T c, T d, T e) : a{a}, b{b}, c{c}, d{d}, e{e} {};
    ConvexGamesSolver(const std::tuple<T, T, T, T, T>& t) {
        auto [a,b,c,d,e] = t;
        *this = ConvexGamesSolver(a, b, c , d, e);
    };
    void solve() {
        std::cout << "Numerical solution: " << '\n';
        size_t N = 2;
        T last_res = 0;
        last_results.reserve(5);
        //while(!is_solving_finished(N)) {
        for (size_t N = 2; N < 120; ++N) {
            std::cout << "N = " << N << '\n';
            auto mesh = create_mesh(N);
            print_matrix(mesh, 10);
            get_saddle_point(mesh);
            if (last_results.size() == 5) last_results[N % 5] = h;
            else last_results.push_back(h);
            if (is_solving_finished(N)) break;
        }
        //h = *std::max_element(last_results.begin(), last_results.end());
    }

    bool solve_analytic() {
        if (2*a >= 0 and 2*b <= 0) {
            std::cout << "game is not convex!" << '\n';
            return false;
        }
        player_B_strategies = (c*d - 2 * a * e)/(4*b*a - c*c);
        player_A_strategies = - (c*player_B_strategies + d) / (2*a);
        h = H(player_A_strategies, player_B_strategies);

        return true;
    }
    [[nodiscard]] std::tuple<T, T, T> get_solution() const {
        return {h, player_A_strategies, player_B_strategies};
    }
private:
    T a, b, c, d, e;
    T player_B_strategies;
    T player_A_strategies;
    T epsilon = 0.04;
    T h;

    std::vector<T> last_results;
    std::vector<T> deltas;

    auto H(T x, T y) -> T {
        return x*x*a + y*y*b + c*x*y + d*x + e*y;
    };

    bool is_solving_finished(size_t N) {
        auto tmp = last_results;
        std::sort(tmp.begin(), tmp.end());
        return std::fabs((tmp[3] - tmp[4])) < epsilon and N > 7;
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
            std::cout << "Saddle point" << '\n';
            size_t i = get_max_element_index(min_row);
            size_t j = get_min_element_index(max_col);
            auto N = m.size() - 1;
            player_A_strategies = static_cast<double>(i)/N;
            player_B_strategies = static_cast<double>(j)/N;
            h = minmax;
            std::cout << "x: " << player_A_strategies << ", y: " << player_B_strategies << ", H: " << h << std::endl;
        }
        else {
            BRMethodSolver<double> br_solver{m, epsilon, 10};
            br_solver.init();
            br_solver.solve(false);
            std::cout << "No saddle point, solution by Brown-Robinson method:" << std::endl;
            auto [gc, player_A_solution, player_B_solution] = br_solver.get_solution();
            std::cout << "x: ";
            print_vector(player_A_solution, 4);
            std::cout << "y: ";
            print_vector(player_B_solution, 4);
            std::cout << "Game Cost v = " << gc << std::endl;
            h = gc;
        }
    }

    matrix create_mesh(size_t N) {
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
};
