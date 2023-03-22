#pragma once
#include <ctime>
#include <map>
#include <random>

#include <matrix_operations.hpp>


template<typename T>
class BRMethodSolver {
public:
    BRMethodSolver(std::vector<std::vector<T>> gm, T de) {
        std::copy(gm.begin(), gm.end(), std::back_inserter(game_matrix));
        size = game_matrix.size();
        playerA_win.resize(size);
        playerB_lose.resize(size);
        desired_epsilon = de;
    }
    void init() {

        for (size_t i = 0; i < size; ++i) {
            strategy_usage_A.insert({i, 0});
            strategy_usage_B.insert({i, 0});
        }
        strategy_usage_A[A_strategy] += 1;
        strategy_usage_B[B_strategy] += 1;

        playerA_win = get_transposed_matrix(game_matrix)[A_strategy];
        playerB_lose = game_matrix[B_strategy];

        upper_game_costs.push_back(compute_upper_game_cost(playerA_win));
        lower_game_costs.push_back(compute_lower_game_cost(playerB_lose));
        epsilon = compute_epsilon(upper_game_costs, lower_game_costs);
    }
    void solve(bool print_info = true) {
        if (print_info) {
            print_header();
            print_step();
        }
        while (epsilon >= desired_epsilon) {
            K++;
            A_strategy = get_new_strategy_A(playerA_win);
            B_strategy = get_new_strategy_B(playerB_lose);
            auto tmp = get_transposed_matrix(game_matrix)[B_strategy];
            std::transform(
                    playerA_win.begin(),
                    playerA_win.end(),
                    tmp.begin(),
                    playerA_win.begin(),
                    std::plus<T>{}
            );
            std::transform(
                    playerB_lose.begin(),
                    playerB_lose.end(),
                    game_matrix[A_strategy].begin(),
                    playerB_lose.begin(),
                    std::plus<T>{}
            );
            upper_game_costs.push_back(compute_upper_game_cost(playerA_win));
            lower_game_costs.push_back(compute_lower_game_cost(playerB_lose));
            strategy_usage_A[A_strategy] += 1;
            strategy_usage_B[B_strategy] += 1;
            epsilon = compute_epsilon(upper_game_costs, lower_game_costs);
            if (print_info) print_step();
        }
        game_cost = compute_game_cost();
        player_A_strategies = compute_strategies(strategy_usage_A);
        player_B_strategies = compute_strategies(strategy_usage_B);
    }
    [[nodiscard]] std::tuple<T, std::vector<T>, std::vector<T>> get_solution() const {
        return {game_cost, player_A_strategies, player_B_strategies};
    }
private:
    T desired_epsilon;
    T epsilon;
    T game_cost;
    size_t size;
    size_t K = 1; // STEP
    size_t A_strategy = 0;
    size_t B_strategy = 0;
    std::vector<T> player_B_strategies;
    std::vector<T> player_A_strategies;
    std::vector<T> playerA_win;
    std::vector<T> playerB_lose;
    std::vector<T> upper_game_costs;
    std::vector<T> lower_game_costs;
    std::map<size_t, size_t> strategy_usage_A;
    std::map<size_t, size_t> strategy_usage_B;
    std::vector<std::vector<T>> game_matrix;

    std::random_device rd;
    std::mt19937 gen{rd()};

    T compute_game_cost() {
        return (get_min_element(upper_game_costs) + get_max_element(lower_game_costs)) / 2;
    }
    auto compute_strategies(const std::map<size_t, size_t>& strategies) {
        std::vector<T> res;
        res.reserve(size);
        for (const auto&[key, value] : strategies) {
            //Q q{static_cast<int32_t>(value)};
            //std::cout << q * Q{1,K} << " ";
            res.push_back(static_cast<double>(value) / K);
        }
        return res;
    }

    T compute_epsilon(const std::vector<T>& ugc, const std::vector<T>& lgc) {
        auto min = get_min_element(ugc);
        auto max = get_max_element(lgc);
        T res = min - max;
        return res;
    }
    T compute_upper_game_cost(const std::vector<T>& vec) {
        return static_cast<double>(get_max_element(vec)) / K;
    }

    T compute_lower_game_cost(const std::vector<T>& vec) {
        return static_cast<double>(get_min_element(vec)) / K;
    }
    size_t get_new_strategy_A(const std::vector<T>& vec, bool manual_input = false) {
        auto max_el = get_max_element(vec);
        std::vector<size_t> max_indexes;
        for (size_t i = 0; i < vec.size(); ++i) if (vec[i] == max_el) max_indexes.push_back(i);

        if (max_indexes.size() == 1) {
            return max_indexes[0];
        }
        else {
            if (manual_input) {
                size_t strat;
                std::cin >> strat;
                return strat;
            }
            std::uniform_int_distribution<size_t> distrib(0, max_indexes.size() - 1);
            return max_indexes[distrib(gen)];
        }
    }

    size_t get_new_strategy_B(const std::vector<T>& vec, bool rand_decide = false) {
        auto min_el = get_min_element(vec);
        std::vector<size_t> min_indexes;
        for (size_t i = 0; i < vec.size(); ++i) if (vec[i] == min_el) min_indexes.push_back(i);

        if (min_indexes.size() == 1) {
            return min_indexes[0];
        }
        else {
            if (rand_decide) {
                size_t strat;
                std::cin >> strat;
                return strat;
            }
            std::uniform_int_distribution<size_t> distrib(0, min_indexes.size() - 1);
            return min_indexes[distrib(gen)];
        }
    }
    void print_header() {
        std::cout << std::setw(3) << "STEP" << " | "
                  << std::setw(3) << "  x " << " | "
                  << std::setw(3) << "  y " << " | ";
        std::cout << std::setw(4*size) << "A wins            " << " | ";
        std::cout << std::setw(4*size) << "B Loses           " << " | ";
        std::cout << std::setw(8) << "UGC" << " | " << std::setw(8) << "LGC" << " | " << "EPSILON";
        std::cout << std::endl;
    }
    void print_step() {
        std::cout << std::setw(4) << K << " | "
                  << std::setw(3) << "x" << A_strategy+1 << " | "
                  << std::setw(3) << "y" << B_strategy+1 << " | ";
        for (T el : playerA_win) std::cout << std::setw(4) << el << " | ";
        for (T el : playerB_lose) std::cout << std::setw(4) << el << " | ";
        std::cout << std::setw(8) << upper_game_costs[K-1] << " | " << std::setw(8) << lower_game_costs[K-1] << " | " << epsilon;
        std::cout << std::endl;
    }
};

//template<>
//Q BRMethodSolver<Q>::compute_upper_game_cost(const std::vector<size_t>& vec, size_t K) {
//    return {static_cast<int32_t>(get_max_element(vec)), K};
//}
//template<>
//Q BRMethodSolver<Q>::compute_lower_game_cost(const std::vector<size_t>& vec, size_t K) {
//    return {static_cast<int32_t>(get_min_element(vec)), K};
//}
//template<>
//auto BRMethodSolver<Q>::compute_strategies(const std::map<size_t, size_t>& strategies) {
//    std::vector<Q> res;
//    res.reserve(size);
//    for (const auto&[key, value] : strategies) {
//        //Q q{static_cast<int32_t>(value)};
//        res.push_back(q * Q{1,K});
//    }
//    return res;
//}