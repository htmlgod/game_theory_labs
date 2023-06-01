#include <algorithm>
#include <numeric>
#include <iostream>
#include <vector>
#include <random>

#include <common/matrix_operations.hpp>

using trust_matrix = std::vector<std::vector<double>>;
    
std::random_device rd; 
std::mt19937 gen(rd());

auto gen_rand_int(int lower_bound, int upper_bound) {
    std::uniform_int_distribution<> distrib(lower_bound, upper_bound);
    return distrib(gen);
}

auto gen_rand_vec_int(int lower_bound, int upper_bound, size_t size = 10) {
    std::vector<double> vec;
    vec.reserve(size);
    for (size_t i = 0; i < size; ++i)
        vec.push_back(static_cast<double>(gen_rand_int(lower_bound, upper_bound)));
    return vec;
}

auto gen_trust_matrix(size_t size = 10) {
    std::random_device rd;
    std::mt19937 gen(rd());
    trust_matrix res;
    res.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        std::vector<double> vec;
        vec.reserve(size);
        for (size_t n = 0; n < size; ++n) {
            vec.push_back(std::generate_canonical<double, 10>(gen));
        }
        auto sum = std::reduce(vec.begin(), vec.end());
        std::for_each(vec.begin(), vec.end(), [=](double& el){ el /= sum; }); 
        res.push_back(vec);
    }
    return res;
}

auto get_xt(const trust_matrix& tm, const std::vector<double>& x0, size_t& iter_count) {
    double epsilon = 0.000001; // check here
    iter_count = 0;
    auto tmp = tm;
    auto xt = x0;
    while (!all_of(xt.begin(), xt.end(), [&] (double i) { return std::fabs(i - xt[0]) < epsilon; })) {
        tmp = matrix_matrix_mul(tmp, tm);
        xt = matrix_vector_mul(tmp, x0);
        iter_count++;
    }
    std::cout << "Resulting trust matrix: \n";
    print_matrix(tmp, 5);
    return xt;
}
auto get_shuffled_indices(size_t size = 10) {
    std::vector<size_t> indices(size);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);

    return indices;
}
void apply_control(std::vector<double>& x0, const std::vector<size_t>& indices, 
                   size_t pl_A_agent_amount, size_t pl_B_agent_amount, double u, double v) {
    for (int i = 0; i < pl_A_agent_amount; ++i) {
        x0[indices[i]] = u;
    }
    for (int i = pl_A_agent_amount; i <= (pl_A_agent_amount + pl_B_agent_amount); ++i) {
        x0[indices[i]] = v;
    }
}

auto main() -> int {

    std::cout << "Trust matrix: \n";
    auto tm = gen_trust_matrix();
    print_matrix(tm, 6,6); // precision here

    std::cout << "\nStarting agents opinions (vector x_0): \n";
    auto x0 = gen_rand_vec_int(1, 20);
    print_vector(x0, 4, 0);

    size_t iter;
    auto xt = get_xt(tm, x0, iter);

    std::cout << "Resulting agents opinions after " << iter << " iterations: \n";
    print_vector(xt);
    
    double u = static_cast<double>(gen_rand_int(0, 100));
    double v = static_cast<double>(gen_rand_int(-100, 0));
    std::cout << "Player A control: u = " << u << '\n';
    std::cout << "Player B control: v = " << v << '\n';

    size_t pl_A_agents_amount = gen_rand_int(1, 8);
    size_t pl_B_agents_amount = gen_rand_int(1, 9 - pl_A_agents_amount);
    std::cout << "Player A agent base: " << pl_A_agents_amount << "\n";
    std::cout << "Player B agent base: " << pl_B_agents_amount << "\n";

    auto x0_controlled = gen_rand_vec_int(1, 20);
    auto indices = get_shuffled_indices();
    apply_control(x0_controlled, indices, pl_A_agents_amount, pl_B_agents_amount, u, v);
    std::cout << "Player A agents: " << "\n";
    for (int i = 0; i < pl_A_agents_amount; ++i) {
        std::cout << indices[i] << ' ';
    }
    std::cout << "\nPlayer B agents: " << "\n";
    for (int i = pl_A_agents_amount; i <= (pl_A_agents_amount + pl_B_agents_amount); ++i) {
        std::cout << indices[i] << ' ';
    }

    std::cout << "\nStarting agents opinions (with control): \n";
    print_vector(x0_controlled);

    iter = 0;
    auto xt_controlled = get_xt(tm, x0_controlled, iter);

    std::cout << "Resulting agents opinions after " << iter << " iterations with control: \n";
    print_vector(xt_controlled);

    std::cout << "Player " << (xt_controlled[0] > 0 ? "A" : "B") << " Won!\n";
    return 0;
}
