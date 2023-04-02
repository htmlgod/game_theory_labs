#include <random>
#include <optional>
#include <set>
#include <stack>
#include <matrix_method.hpp>

template <typename T>
class SadovskyIterMethodSolver {
    using matrix = std::vector<std::vector<T>>;
    using vec = std::vector<T>;
public:
    explicit SadovskyIterMethodSolver(const matrix& m) {
        std::copy(m.begin(), m.end(), std::back_inserter(game_matrix));
        size = m.size();
    }
    // iter 0: select random x strat
    void init() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> distrib(0, game_matrix.size() - 1);
        auto first_strategy = distrib(gen);
        //auto first_strategy = 0;
        print_matrix(game_matrix);

        x_prev = vec(size, 0);
        c_prev = game_matrix[first_strategy];
        x_prev[first_strategy] = 1;
        lower_game_cost = get_min_element(c_prev);
        J.insert(get_min_element_index(c_prev));
        std::cout << "Iteration " << ITERATION << std::endl;
        std::cout << "x0 ";
        print_vector(x_prev);
        std::cout << "c0 ";
        print_vector(c_prev);
    }
    void solve() {
        init();
        vec x_iter; // x~N
        vec c_iter; // c~N
        for (;;) {
            ++ITERATION;
            std::cout << "Iteration " << ITERATION << std::endl;
            std::cout << "Subgame:" << std::endl;
            auto subgame = get_subgame();

            print_matrix(subgame);
            if (subgame.size() == 1) { // solve 1 dimensional subgame
                auto tmp = get_transposed_matrix(subgame);
                x_iter = vec(size, 0);
                auto strategy = get_max_element_index(tmp);
                std::cout << strategy << std::endl;
                x_iter[strategy] = 1;
                c_iter = game_matrix[strategy];
            }
            else {
                if (subgame.size() > subgame[0].size()) subgame = get_transposed_matrix(subgame);
                auto indexes = reduce_game_matrix(subgame);
                print_matrix(subgame);
                auto solution = solve_2x2subgame(subgame);
                std::stack<T> s;
                s.push(solution[1]);
                s.push(solution[0]);
                x_iter.resize(size);
                for (size_t i = 0; i < size; ++i) {
                    if (indexes.count(i) > 0) {
                        x_iter[i] = s.top();
                        s.pop();
                    }
                }
                print_vector(x_iter);
                c_iter = compute_c_iter(x_iter);
            }
            print_vector(c_iter);
            print_vector(x_iter);
            matrix game2xn = {
                    c_prev,
                    c_iter
            };
            print_matrix(game2xn);
            // check before reducing, if one row is dominating
            std::vector<T> solution2x2;
            if (is_2xn_game_has_dominant_row(game2xn)) {
                solution2x2 = {0, 1};
            }
            else {
                auto indexes = reduce_game_matrix(game2xn);
                print_matrix(game2xn);
                solution2x2 = solve_2x2subgame(game2xn);
            }

            if (solution2x2[0] == 0 or solution2x2[1] == 0) break;
            x_current = number_vector_mul(solution2x2[0], x_prev) + number_vector_mul(solution2x2[1], x_iter);
            c_current = number_vector_mul(solution2x2[0], c_prev) + number_vector_mul(solution2x2[1], c_iter);
            print_vector(x_current);
            print_vector(c_current);
            x_prev = x_current;
            c_prev = c_current;
            lower_game_cost = get_min_element(c_current);
            std::cout << lower_game_cost << std::endl;
            J.insert(get_min_element_index(c_current));
        }
    }
    [[nodiscard]] std::tuple<T, std::vector<T>> get_solution() const {
        return {lower_game_cost, x_prev};
    }
private:
    vec compute_c_iter(vec x_iter) {
        vec res = vec(x_iter.size(), 0);
        for (size_t i = 0; i < x_iter.size(); ++i) {
            res = res + number_vector_mul(x_iter[i], game_matrix[i]);
        }
        return res;
    }
    size_t ITERATION = 0;
    std::set<size_t> J;
    matrix game_matrix;
    size_t size;
    vec x_current;
    vec x_prev;
    vec c_current;
    vec c_prev;
    T lower_game_cost = 0;

    void solve_subgame() {

    }

    void solve_2xn_game() {

    }

    matrix get_subgame() {
        auto tmp = get_transposed_matrix(game_matrix);
        matrix res;
        for (auto col : J) {
            res.push_back(tmp[col]);
        }
        return res;
    }
    std::vector<T> solve_2x2subgame(const matrix& m) {
//        MatrixMethodSolver<double> m_solver{m};
//        m_solver.init();
//        m_solver.solve();
//        auto[gc, player_A_solution, player_B_solution] = m_solver.get_solution(0);
//        return player_A_solution;
        auto x1 = (m[1][1] - m[1][0]) / (m[0][0] - m[1][0] - m[0][1] + m[1][1]);
        auto x2 = 1 - x1;
        return {x1,x2};
    }
    bool is_2xn_game_has_dominant_row(const matrix& gm) {
        bool r1_dom_r2 = std::equal(gm[0].begin(), gm[0].end(), gm[1].begin(), [](T el1, T el2) { return el1 >= el2; });
        bool r2_dom_r1 = std::equal(gm[1].begin(), gm[1].end(), gm[0].begin(), [](T el1, T el2) { return el1 >= el2; });
        return (r1_dom_r2 or r2_dom_r1);
    }

    size_t get_nbr_row(const matrix& t_gm) { // t_gm == transposed
        std::set<size_t> indexes;
        for (size_t i = 0; i < t_gm[0].size(); ++i) {
            indexes.insert(i);
        }
        for (size_t i = 0; i < t_gm.size(); ++i) {
            indexes.erase(get_max_element_index(t_gm[i]));
        }
        return *indexes.begin();
    }

    size_t get_nbr_col(const matrix& gm) {
        std::set<size_t> indexes;
        for (size_t i = 0; i < gm[0].size(); ++i) {
            indexes.insert(i);
        }
        for (size_t i = 0; i < gm.size(); ++i) {
            indexes.erase(get_min_element_index(gm[i]));
        }
        return *indexes.begin();
    }

    std::set<size_t> reduce_game_matrix(matrix& gm) {
        std::set<size_t> indexes;
        for (size_t i = 0; i < gm.size(); ++i) {
            indexes.insert(i);
        }
        std::optional<size_t> found_index = {};
        for (;;) {
            // cols > rows
            if (gm.size() == 2 and gm[0].size() == 2) break;
            if (gm[0].size() >= gm.size()) {
                gm = get_transposed_matrix(gm);
                print_matrix(gm);
                for (size_t i = 0; i < gm.size(); ++i) {
                    for (size_t j = 0; j < gm.size(); ++j) {
                        if (i != j) {
                            bool can_reduce = std::equal(gm[i].begin(), gm[i].end(), gm[j].begin(), [](double el1, double el2) { return el1 <= el2; });
                            if (can_reduce) {
                                found_index = j;
                                break;
                            }
                        }
                    }
                    if (found_index) break;
                }
                if (found_index) {
                    gm.erase(gm.begin() + found_index.value());
                    found_index.reset();
                }
                else {
                    auto ind = get_nbr_col(gm);
                    std::cout << "nbr" << std::endl;
                    gm.erase(gm.begin() + ind);
                }
                gm = get_transposed_matrix(gm);
            }
                // rows > cols
            else if (gm[0].size() < gm.size()) {
                for (size_t i = 0; i < gm.size(); ++i) {
                    for (size_t j = 0; j < gm.size(); ++j) {
                        if (i != j) {
                            bool can_reduce = std::equal(gm[i].begin(), gm[i].end(), gm[j].begin(), [](double el1, double el2) { return el1 >= el2; });
                            if (can_reduce) {
                                found_index = j;
                                break;
                            }
                        }
                    }
                    if (found_index) break;
                }
                if (found_index) {
                    gm.erase(gm.begin() + found_index.value());
                    indexes.erase(found_index.value());
                    found_index.reset();
                }
                else {
                    gm = get_transposed_matrix(gm);
                    auto ind = get_nbr_row(gm);
                    indexes.erase(ind);
                    gm = get_transposed_matrix(gm);
                    gm.erase(gm.begin() + ind);
                }
            }
        }
        return indexes;
    }
};

auto main() -> int {
    std::vector<std::vector<double>> A = {
            {8, 12, 10},
            {1, 6, 19},
            {17, 11, 11}
    };
    SadovskyIterMethodSolver solver(A);
    solver.solve();
    auto [gc, strat] = solver.get_solution();

    std::cout << "Solution: " << std::endl;
    std::cout << "Game cost: " << gc << std::endl;
    std::cout << "Player A strat: ";
    print_vector(strat);
}
