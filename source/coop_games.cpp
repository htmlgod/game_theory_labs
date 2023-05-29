#include <algorithm>
#include <cmath>
#include <bitset>
#include <iostream>
#include <numeric>
#include <vector>
#include <common/matrix_operations.hpp>
//#include <ranges>


constexpr size_t total_players = 4;

auto compute_factorial(int N) {
    if (N < 0) N = 0;
    return std::tgamma(N+1);
}

size_t get_coalition_size(size_t c) {
    return std::bitset<total_players>(c).count();
}

//auto compute_shapley_for_player(size_t player) {
//    auto is_present = [=](size_t i) {
//        return i & (1 << player);
//    }
//    double result = 0;
//    for (size_t i : std::ranges::iota(0, total_players) | std::views::filter(is_present)) {
//        result += compute_factorial(get_coalition_size(i)-1)*compute_factorial(total_players-get_coalition_size(i))*(CF[i]-CF[i ^ 1 << player]);
//    }
//
//    return result / compute_factorial(total_players);
//}

auto compute_shapley_for_player(size_t player, const std::vector<int>& CF, size_t amount_of_coalitions) {
    std::vector<int> all_coalitions(amount_of_coalitions);
    std::iota(all_coalitions.begin(), all_coalitions.end(), 0);
    std::vector<size_t> player_coalitions;
    auto is_present = [=](size_t i) {
        return (i & (1 << player));
    };
    std::copy_if(all_coalitions.begin(), all_coalitions.end(), std::back_inserter(player_coalitions), is_present);
    double result = 0;
    for (size_t i : player_coalitions) {
        result += compute_factorial(get_coalition_size(i)-1)*compute_factorial(total_players-get_coalition_size(i))*(CF[i]-CF[i ^ 1 << player]);
    }

    return result / static_cast<double>(compute_factorial(total_players));
}

auto is_superadditive(const std::vector<int>& CF, size_t amount_of_coalitions, size_t& broken_t, size_t& broken_s) {
    for (int s = 0; s < amount_of_coalitions; ++s) {
        for (int t = 0; t < amount_of_coalitions; ++t) {
            if ((s & t) != 0) continue;
            if (CF[s | t] < (CF[s] + CF[t])) {
                broken_t = t;
                broken_s = s;
                return false;
            }
        }
    }
    return true;
}

auto is_convex(const std::vector<int>& CF, size_t amount_of_coalitions) {
    for (int s = 0; s < amount_of_coalitions; ++s) {
        for (int t = 0; t < amount_of_coalitions; ++t) {
            if ((CF[s | t] + CF[s & t]) < (CF[s] + CF[t])) return false;
        }
    }
    return true;
}

auto main() -> int {
    constexpr size_t amount_of_coalitions = 1 << total_players;
    std::vector<int> CF(amount_of_coalitions);
    CF[0x0] = 0; 
    CF[0x1] = 3; 
    CF[0x2] = 2; 
    CF[0x4] = 3; 
    CF[0x8] = 2; 
    CF[0x3] = 6; 
    CF[0x5] = 6; 
    CF[0x9] = 5; 
    CF[0x6] = 6; 
    CF[0xA] = 4; 
    CF[0xC] = 7; 
    CF[0x7] = 10; 
    CF[0xB] = 10; 
    CF[0xD] = 9;
    CF[0xE] = 10; 
    CF[0xF] = 12;
    size_t broken_t = 0;
    size_t broken_s = 0;
    auto is_supadd = is_superadditive(CF, amount_of_coalitions, broken_t, broken_s);
    std::cout << "Game properties:\n";
    std::cout << "\tis" << (is_supadd ? "" : " not") << " Superadditive\n";
    std::cout << "\tis" << (is_convex(CF, amount_of_coalitions) ? "" : " not") << " Convex\n";

    while (!is_superadditive(CF, amount_of_coalitions, broken_t, broken_s)) {
        std::vector<size_t> CS = {
            broken_s,
            broken_t
        };
        CF[broken_s | broken_t] = *std::max_element(CS.begin(), CS.end());
        broken_t = 0;
        broken_s = 0;
    }
    is_supadd = is_superadditive(CF, amount_of_coalitions, broken_t, broken_s);
    std::cout << "Game properties (after transformation into superadditive):\n";
    std::cout << "\tis" << (is_supadd ? "" : " not") << " Superadditive\n";
    std::cout << "\tis" << (is_convex(CF, amount_of_coalitions) ? "" : " not") << " Convex\n";

    std::cout << "\n\nShapley Vector: ";
    std::vector<double> sv(total_players);
    std::cout << "(";
    for (size_t i = 0; i < total_players; ++i) {
        sv[i] = compute_shapley_for_player(i, CF, amount_of_coalitions);
        std::cout << sv[i] << " ";
    }
    std::cout << ")\n\n\n";

    auto check_group_rationalisation = [](std::vector<double> sv, std::vector<int> CF) {
        return CF[0xF] == std::reduce(sv.begin(),sv.end());
    };
    std::cout << "Game conditions:\n";
    std::cout << "\tgroup rationalisation is" << (check_group_rationalisation(sv, CF) ? "" : " not") << " met\n";
    std::cout << "\tindividual rationalisation:\n";
    for (size_t i = 0; i < total_players; ++i) {
        std::cout << "\t\tPlayer " << i << ": condition is" << ( sv[i] > CF[1 << i] ? "" : " not") << " met\n";
    }

    return 0;
}
