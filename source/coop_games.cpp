#include <algorithm>
#include <cmath>
#include <string>
#include <bitset>
#include <iostream>
#include <numeric>
#include <vector>
#include <map>
#include <common/matrix_operations.hpp>

constexpr size_t total_players = 4;

auto compute_factorial(int N) {
    if (N < 0) N = 0;
    return std::tgamma(N+1);
}

size_t get_coalition_size(size_t c) {
    return std::bitset<total_players>(c).count();
}

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

auto compute_shapley_vec(const std::vector<int>& CF, size_t amount_of_coalitions) {
    std::vector<double> sv(total_players);
    for (size_t i = 0; i < total_players; ++i) {
        sv[i] = compute_shapley_for_player(i, CF, amount_of_coalitions);
    }
    return sv;

}

std::string decode_coalition(int c) {
    if (c == 0) return "(0)";
    std::bitset<total_players> bitset(c);
    std::string res = "";
    int commas = bitset.count() -1 ;
    for (size_t i = 0; i < total_players; ++i) {
        if (bitset[i]) {
            res += std::to_string(total_players - i);
            if (commas != 0) {
                --commas;
                res += ", ";
            }
        }
    }
    if (!res.empty()) res = "(" + res + ")";
    return res;
}

auto check_superadditivity(const std::vector<int>& CF, size_t amount_of_coalitions, std::multimap<size_t, std::vector<size_t>>& CS, bool print = false) {
    auto result = true;
    for (int s = 0; s < amount_of_coalitions; ++s) {
        for (int t = 0; t < amount_of_coalitions; ++t) {
            if ((s & t) != 0) continue;
            bool check = CF[s | t] < (CF[s] + CF[t]);
            if (check) {
                if (result) std::cout << "Supperadditivity (failed checks):\n";
                result = false;
                if (print) {
                    std::cout << "CF[" << decode_coalition(s | t) << "] ";
                    std::cout << "< GF[" <<decode_coalition(s) << "] + GF[" << decode_coalition(t) << "]\n";
                }
                CS.insert({s | t, {static_cast<size_t>(CF[s]), static_cast<size_t>(CF[t])}});
            }
        }
    }
    std::cout << "";
    return result;
}

auto check_convex(const std::vector<int>& CF, size_t amount_of_coalitions, bool print = true) {
    auto result = true;
    for (int s = 0; s < amount_of_coalitions; ++s) {
        for (int t = 0; t < amount_of_coalitions; ++t) {
            bool check = (CF[s | t] + CF[s & t]) < (CF[s] + CF[t]);
            if (check) {
                if (result) std::cout << "Convex (failed checks):\n";
                result = false;
                if (print) {
                    std::cout << "CF[" << decode_coalition(s | t) << "] ";
                    std::cout << " + CF[" << decode_coalition(s & t) << "] ";
                    std::cout << " < CF[" << decode_coalition(s) << "] + CF[" << decode_coalition(t) << "]\n";
                }
            }
        }
    }
    return result;
}

void transform(std::vector<int>& CF, const std::multimap<size_t, std::vector<size_t>>& CS) {
    std::map<size_t, std::vector<size_t>> sums;
    std::cout << "Transforming to superadditive game using these coalition structures:\n";
    for (const auto& [T, CSt] : CS) {
        std::cout << decode_coalition(T) << " : ";
        for (auto coalition : CSt) {
            std::cout << decode_coalition(coalition) << " ";
        }
        std::cout << '\n';
    }
    for (const auto& [T, CSt] : CS) {
        sums[T].push_back(std::reduce(CSt.cbegin(), CSt.cend()));
    }
    for (const auto& [T, CSt] : sums) {
        CF[T] = *std::max_element(CSt.cbegin(), CSt.cend());
    }
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

    std::cout << "Initial CF vector:\n";
    std::cout << std::string(75, '=')  << '\n';
    print_vector(CF);


    std::cout << "\nChecking properties:\n";
    std::cout << std::string(75, '=')  << '\n';

    std::multimap<size_t, std::vector<size_t>> cs;
    auto is_supadd = check_superadditivity(CF, amount_of_coalitions, cs, true);
    std::cout << std::string(75, '=')  << '\n';
    auto is_convex = check_convex(CF, amount_of_coalitions);
    std::cout << "Game properties after check:\n";
    std::cout << "\tis" << (is_supadd ? "" : " not") << " Superadditive\n";
    std::cout << "\tis" << (is_convex ? "" : " not") << " Convex\n";

    std::cout << "\nTransform non-superadditive game to superadditive:\n";
    std::cout << std::string(75, '=')  << '\n';
    transform(CF, cs);

    std::cout << "\nChecking properties after transform:\n";
    std::cout << std::string(75, '=')  << '\n';
    is_supadd = check_superadditivity(CF, amount_of_coalitions, cs);
    is_convex = check_convex(CF, amount_of_coalitions);
    std::cout << "Game properties after check:\n";
    std::cout << "\tis" << (is_supadd ? "" : " not") << " Superadditive\n";
    std::cout << "\tis" << (is_convex ? "" : " not") << " Convex\n";
    std::cout << "\nCF vector after transform:\n";
    std::cout << std::string(75, '=')  << '\n';
    print_vector(CF);

    std::cout << "\n\nShapley Vector: ";
    auto sv = compute_shapley_vec(CF, amount_of_coalitions);
    print_vector(sv);
    std::cout << std::string(75, '=')  << '\n';


    std::cout << "\nGame conditions:\n";
    std::cout << std::string(75, '=')  << '\n';

    std::cout << "Group rationalisation:\n";
    std::cout << "CF[(4,3,2,1)] == " << std::reduce(sv.begin(),sv.end()) << ";\n";
    std::cout << CF[0xF] << " == " << std::reduce(sv.begin(), sv.end()) << '\n';

    std::cout << "\nIndividual rationalisation:\n";
    for (size_t i = 0; i < total_players; ++i) {
        auto ir = sv[i] >= CF[1 << i];
        std::cout << "\tPlayer " << i << ": condition is" << ( ir  ? "" : " not") << " met\n";
        if (!ir) {
            std::cout << "\tx" << i << "(CF) < " << "CF[(" << i << ")]\n";
        }
    }
    return 0;
}
