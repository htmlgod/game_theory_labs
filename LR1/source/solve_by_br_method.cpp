#include <cstdlib>
#include <ctime>
#include <map>

#include <matrix_operations.hpp>
#include <quotient.hpp>


size_t get_max_element_index(const std::vector<size_t>& vec) {
    return std::distance(vec.begin(), std::max_element(vec.begin(), vec.end()));
}

size_t get_min_element_index(const std::vector<size_t>& vec) {
    return std::distance(vec.begin(), std::min_element(vec.begin(), vec.end()));
}

template<typename T>
T get_max_element(const std::vector<T>& vec) {
    return *std::max_element(vec.begin(), vec.end());
}

template<typename T>
T get_min_element(const std::vector<T>& vec) {
    return *std::min_element(vec.begin(), vec.end());
}

size_t get_new_strat_A(const std::vector<size_t>& vec, bool rand_decide = false) {
    auto max_el = get_max_element(vec);
    std::vector<size_t> max_indexes;
    for (size_t i = 0; i < vec.size(); ++i) if (vec[i] == max_el) max_indexes.push_back(i);

    if (max_indexes.size() == 1) {
        return max_indexes[0];
    }
    else {
        if (rand_decide) {
            size_t strat;
            std::cin >> strat;
            return strat;
        }
        return max_indexes[std::rand() % max_indexes.size()];
    }
}

size_t get_new_strat_B(const std::vector<size_t>& vec, bool rand_decide = false) {
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
        return min_indexes[std::rand() % min_indexes.size()];
    }
}

Q compute_upper_game_cost(const std::vector<size_t>& vec, size_t K) {
    return {static_cast<int32_t>(get_max_element(vec)), K};
}

Q compute_lower_game_cost(const std::vector<size_t>& vec, size_t K) {
    return {static_cast<int32_t>(get_min_element(vec)), K};
}

Q compute_epsilon(const std::vector<Q>& ugc, const std::vector<Q>& lgc) {
    auto min = get_min_element(ugc);
    //std::cout << "eps_dbg: min=" << min << " , max= ";
    auto max = get_max_element(lgc);
    //std::cout << max;
    Q res = min - max;
    //std::cout << ", min-max= " << res << std::endl;
    return res;
}

void print_step(size_t step, size_t stratA, size_t stratB, const std::vector<size_t>& Awin, const std::vector<size_t>& Blose, Q ugc, Q lgc, Q epsilon) {
    std::cout << std::setw(3) << step << " | "
              << std::setw(3) << "x" << stratA+1 << " | "
              << std::setw(3) << "y" << stratB+1 << " | ";
    for (size_t el : Awin) std::cout << std::setw(4) << el << " | ";
    for (size_t el : Blose) std::cout << std::setw(4) << el << " | ";
    std::cout << std::setw(8) << ugc << " | " << std::setw(8) << lgc << " | " << epsilon;
    std::cout << std::endl;
}

auto main(int argc, char** argv) -> int {
    std::srand(std::time(nullptr));
    std::cout << "### Matrix game solver (BR method)" << '\n';
    std::cout << "Enter matrix size (only n): ";
    size_t size;
    std::cin >> size;

    std::vector<std::vector<size_t>> game_matrix;
    std::cout << "Enter game matrix: ";
    fill(game_matrix, size);

    std::cout << "Entered game matrix:" << std::endl;
    print_matrix(game_matrix, 1);


    std::vector<size_t> playerA_win{size};
    std::vector<size_t> playerB_lose{size};

    std::vector<Q> upper_game_costs;
    std::vector<Q> lower_game_costs;
    std::map<size_t, size_t> strat_usageA;
    std::map<size_t, size_t> strat_usageB;
    for (size_t i = 0; i < size; ++i) {
        strat_usageA[i] = 0;
        strat_usageB[i] = 0;
    }

    // solving for player A
    size_t K = 1;
    size_t A_strat = 0;
    size_t B_strat = 0;
    strat_usageA[A_strat] += 1;
    strat_usageB[B_strat] += 1;
    playerA_win = get_transposed_matrix(game_matrix)[A_strat];
    playerB_lose = game_matrix[B_strat];
    upper_game_costs.push_back(compute_upper_game_cost(playerA_win, K));
    lower_game_costs.push_back(compute_lower_game_cost(playerB_lose,K));
    Q eps;
    Q desired_epsilon{1,10};
    eps = compute_epsilon(upper_game_costs, lower_game_costs);
    print_step(K, A_strat, B_strat, playerA_win, playerB_lose, upper_game_costs[K-1], lower_game_costs[K-1], eps);
    while (eps >= desired_epsilon) {

        // print_vector(upper_game_costs);
        // print_vector(lower_game_costs);
        K++;
        A_strat = get_new_strat_A(playerA_win);
        B_strat = get_new_strat_B(playerB_lose);
        auto tmp = get_transposed_matrix(game_matrix)[B_strat];
        std::transform(
                playerA_win.begin(), 
                playerA_win.end(), 
                tmp.begin(), 
                playerA_win.begin(), 
                std::plus<size_t>{}
        );
        std::transform(
                playerB_lose.begin(), 
                playerB_lose.end(), 
                game_matrix[A_strat].begin(),
                playerB_lose.begin(), 
                std::plus<size_t>{}
        );
        upper_game_costs.push_back(compute_upper_game_cost(playerA_win,K));
        lower_game_costs.push_back(compute_lower_game_cost(playerB_lose,K));
        strat_usageA[A_strat] += 1;
        strat_usageB[B_strat] += 1;
        eps = compute_epsilon(upper_game_costs, lower_game_costs);
        print_step(K, A_strat, B_strat, playerA_win, playerB_lose, upper_game_costs[K-1], lower_game_costs[K-1], eps);
    }
    for (const auto&[key, value] : strat_usageA) {
        Q q{static_cast<int32_t>(value)};
        std::cout << q * Q{1,K} << " ";

    }
    std::cout << std::endl;
    for (const auto&[key, value] : strat_usageB) {
        Q q{static_cast<int32_t>(value)};
        std::cout << q * Q{1,K} << " ";
    }
    std::cout << std::endl;
}
