#pragma once
#include <tuple>

#include <quotient.hpp>
#include <matrix_operations.hpp>

template <typename T>
class MatrixMethodSolver {
public:
    explicit MatrixMethodSolver(const std::vector<std::vector<T>>& matrix) {
        std::copy(matrix.begin(), matrix.end(), std::back_inserter(game_matrix));
        size = matrix.size();
    }
    void init() {
        u = std::vector<T>(size, T{1});
        inverse_game_matrix = get_inversed_matrix(game_matrix);
    }
    auto get_u_vector() const {
        if (u.empty()) throw std::logic_error{"MatrixMethodSolver not initialised!"};
        return u;
    }
    auto get_common_value() const {
        if (common_value == T{}) throw std::logic_error{"Call solve() method first"};
        return common_value;
    }
    auto get_solution_playerB() const {
        if (player_A_strategies.empty()) throw std::logic_error{"Call solve() method first"};
        return player_A_strategies;
    }
    auto get_solution_playerA() const {
        if (player_B_strategies.empty()) throw std::logic_error{"Call solve() method first"};
        return player_B_strategies;
    }
    auto get_gamecost(const T& delta) const {
        if (common_value == T{}) throw std::logic_error{"Call solve() method first"};
        return (1./common_value) - delta;
    }
    void solve() {
        compute_common_value();
        compute_player_A_strategies();
        compute_player_B_strategies();
    }
    [[nodiscard]] std::tuple<T, std::vector<T>, std::vector<T>> get_solution(T delta) const {
        return {get_gamecost(delta), player_B_strategies, player_A_strategies};
    }
private:
    void compute_common_value() {
        auto buf = vector_matrix_mul(u, inverse_game_matrix);
        common_value = vector_vector_mul(buf, u);
    }
    void compute_player_A_strategies() {
        player_A_strategies = vector_matrix_mul(u, inverse_game_matrix);
        player_A_strategies = vector_number_div(common_value, player_A_strategies);
    }
    void compute_player_B_strategies() {
        player_B_strategies = matrix_vector_mul(inverse_game_matrix, u);
        player_B_strategies = vector_number_div(common_value, player_B_strategies);
    }
    std::vector<std::vector<T>> game_matrix;
    size_t size;
    std::vector<T> u;
    std::vector<std::vector<T>> inverse_game_matrix;
    std::vector<T> player_A_strategies;
    std::vector<T> player_B_strategies;
    T common_value;
};
template<>
auto MatrixMethodSolver<Q>::get_gamecost(const Q& delta) const {
    if (common_value == Q{}) throw std::logic_error{"Call solve() method first"};
    return (common_value - delta).inverse();
}
