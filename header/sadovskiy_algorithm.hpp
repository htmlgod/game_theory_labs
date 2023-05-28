#include <random>
#include <stack>

#include <common/matrix_operations.hpp>

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
        std::cout << "x0 = ";
        print_vector(x_prev);
        std::cout << "c0 = ";
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

            // SOLVING SUBGAME
            if (subgame[0].size() == 1) { // solve 1 dimensional subgame
                auto tmp = get_transposed_matrix(subgame);
                x_iter = vec(size, 0);
                auto strategy = get_max_element_index(tmp[0]);
                x_iter[strategy] = 1;
                c_iter = game_matrix[strategy];
            }
            else {
                auto removed_rows_indexes = reduce_gm_to_2x2(subgame);
                auto solution = solve_2x2game(subgame);
                std::stack<T> s;
                std::cout << "Subgame solution: ";
                print_vector(solution);
                s.push(solution[1]);
                s.push(solution[0]);
                x_iter.resize(size);
                for (size_t i = 0; i < size; ++i) {
                    if (removed_rows_indexes.count(i) == 0) {
                        x_iter[i] = s.top();
                        s.pop();
                    }
                }
                c_iter = compute_c_iter(x_iter);
            }
            std::cout << "c~" << ITERATION << " = ";
            print_vector(c_iter);
            std::cout << "x~" << ITERATION << " = ";
            print_vector(x_iter);

            // SOLVING 2xN GAME
            matrix game2xn = {
                    c_prev,
                    c_iter
            };
            std::cout << "Solving 2xN game: " << '\n';
            print_matrix(game2xn);
            // check before reducing, if one row is dominating

            std::vector<T> solution2x2(2,1);
            if (!get_dominated_rows_indexes(game2xn).empty()) {
                solution2x2[*get_dominated_rows_indexes(game2xn).begin()] = 0;
            }
            else {
                reduce_gm_to_2x2(game2xn);
                if (game2xn.size() != game2xn[0].size()) {
                    if (!get_dominated_rows_indexes(game2xn).empty()) {
                        solution2x2[*get_dominated_rows_indexes(game2xn).begin()] = 0;
                    }
                }
                else {
                    solution2x2 = solve_2x2game(game2xn);
                }
            }

            if (std::any_of(solution2x2.cbegin(), solution2x2.cend(), [](T i){ return i == 0; })) break;
            x_current = number_vector_mul(solution2x2[0], x_prev) + number_vector_mul(solution2x2[1], x_iter);
            c_current = number_vector_mul(solution2x2[0], c_prev) + number_vector_mul(solution2x2[1], c_iter);
            std::cout << "x" << ITERATION << " = ";
            print_vector(x_current);
            std::cout << "c" << ITERATION << " = ";
            print_vector(c_current);
            x_prev = x_current;
            c_prev = c_current;
            lower_game_cost = get_min_element(c_current);
            for (size_t i = 0; i < c_current.size(); ++i) {
                if (c_current[i] == lower_game_cost) {
                    J.insert(i);
                }
            }
            std::cout << "V = ";
            std::cout << lower_game_cost << std::endl;
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
    matrix get_subgame() {
        auto tmp = get_transposed_matrix(game_matrix);
        matrix res;
        for (auto col : J) {
            res.push_back(tmp[col]);
        }
        return get_transposed_matrix(res);
    }
    std::vector<T> solve_2x2game(const matrix& m) {
        auto x1 = (m[1][1] - m[1][0]) / (m[0][0] - m[1][0] - m[0][1] + m[1][1]);
        auto x2 = 1 - x1;
        return {x1,x2};
    }

    std::set<size_t> reduce_gm_to_2x2(matrix& gm) {
        std::set<size_t> row_indexes;
        for (;;) {
            if (gm.size() <= 2 and gm[0].size() <= 2) break;
            // cols >= rows
            if (gm[0].size() >= gm.size()) {
                auto dom_indexes = get_dominated_cols_indexes(gm);
                if (dom_indexes.empty()) {
                    dom_indexes = get_nbr_cols_indexes(gm);
                }
                if (!dom_indexes.empty()) {
                    gm = get_reduced_matrix_by_indexes(gm, dom_indexes, true);
                }
            }
                // rows > cols
            else {
                auto dom_indexes = get_dominated_rows_indexes(gm);
                if (!dom_indexes.empty()) {
                    row_indexes.merge(dom_indexes);
                }
                else {
                    dom_indexes = get_nbr_rows_indexes(gm);
                    row_indexes.merge(dom_indexes);
                }
                if (!dom_indexes.empty()) {
                    gm = get_reduced_matrix_by_indexes(gm, dom_indexes);
                }
            }
        }
        return row_indexes;
    }
};