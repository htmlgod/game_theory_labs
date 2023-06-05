#include <common/matrix_operations.hpp>
#include <string>
#include <sstream>
#include <random>


// COLORS FOR PARETO, NASH AND BOTH
std::ostream& bold_on(std::ostream& os)
{
    return os << "\e[1m";
}
std::ostream& bold_off(std::ostream& os)
{
    return os << "\e[0m";
}
std::ostream& red_on(std::ostream& os) {
    return os << "\x1B[31m";
}
std::ostream& red_off(std::ostream& os) {
    return os << "\033[0m";
}

template<typename T>
void pretty_print(T A, T B, bool is_nash_opt, bool is_pareto_opt, size_t setw = 4, size_t el_setw = 3) {
    if (is_nash_opt) std::cout << bold_on;
    if (is_pareto_opt) std::cout << red_on;
    std::ostringstream oss;
    oss << '(' << std::setw(el_setw) << A << '|' << std::setw(el_setw) << B << ')';
    std::cout << std::setw(setw) << oss.str();
    if (is_pareto_opt) std::cout << red_off;
    if (is_nash_opt) std::cout << bold_off;
}

void solve_mixed(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B) {
    std::vector<double> u(A.size(), 1);

    auto inv_A = get_inversed_matrix(A);
    auto tmp = vector_matrix_mul(u,inv_A);
    double v_1 = 1./vector_vector_mul(tmp, u);

    auto inv_B = get_inversed_matrix(B);
    tmp = vector_matrix_mul(u, inv_B);
    double v_2 = 1./vector_vector_mul(tmp, u);

    std::vector<double> x = number_vector_mul(v_2, vector_matrix_mul(u, inv_B));
    std::vector<double> y = number_vector_mul(v_1, matrix_vector_mul(inv_A, u));
    std::cout << "x = ";
    print_vector(x, 3);
    std::cout << "y = ";
    print_vector(y, 3);
    std::cout << "v1 = " << v_1 << "; v2 = " << v_2 << '\n';
}

template<typename T>
bool check_pareto(size_t io, size_t jo, const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B) {
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < B.front().size(); ++j) {
            if ((A[i][j] >= A[io][jo] and B[i][j] >= B[io][jo]) and (A[i][j] > A[io][jo] and B[i][j] > B[io][jo])) return false;
        }
    }
    return true;
}

template<typename T>
bool check_nash(size_t io, size_t jo, const std::vector<std::vector<T>>& A, const std::vector<std::vector<T>>& B) {
    auto A_t = get_transposed_matrix(A);
    return jo == get_max_element_index(B[io]) and io == get_max_element_index(A_t[jo]);

}

auto gen_rand_gamematrix(size_t seed = 0, size_t row_size = 10, size_t col_size = 10, int lower_bound = -50, int upper_bound = 50) {
    std::vector<std::vector<int>> gm;
    gm.resize(row_size);
    for (auto& row : gm) row.resize(col_size);
    std::random_device rd;
    if (seed == 0) auto seed = rd();
    std::cout << "seed = " << seed << '\n';
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(lower_bound, upper_bound);
    for (auto& row : gm) {
        for (auto& el : row) {
            el = distrib(gen);
        }
    }
    return gm;
}

int main(int argc, const char * argv[]) {
    std::cout << std::string(60, '=') << '\n';
    std::cout << "Hint: " << red_on << "Pareto optimal" << red_off << ", " << bold_on << "Nash equilibrium" << bold_off << ", " << red_on << bold_on << "Both" << red_off << bold_off << '\n' << '\n'; 
    {
        auto A = gen_rand_gamematrix(3161171797);
        auto B = gen_rand_gamematrix(2155087560);
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < B.front().size(); ++j) {
                pretty_print(A[i][j], B[i][j], check_nash<int>(i,j,A,B), check_pareto(i,j,A,B), 10);
            }
            std::cout << '\n';
        }
    }
    {
        std::cout << std::string(60, '=') << '\n';
        std::cout << "Crossroad " << '\n';
        double epsA = 0.1;
        double epsB = 0.4;
        std::cout << "epsilonA = " << epsA << '\n';
        std::cout << "epsilonB = " << epsB << '\n';
        std::vector<std::vector<double>> A = {
            {-1, 1-epsA},
            {2, 0}
        };
        std::vector<std::vector<double>> B = {
            {-1, 2},
            {1-epsB, 0}
        };
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < A.front().size(); ++j) {
                pretty_print(A[i][j], B[i][j], check_nash(i,j,A,B), check_pareto(i,j,A,B), 5);
            }
            std::cout << '\n';
        }
    }
    {
        std::cout << std::string(60, '=') << '\n';
        std::cout << "Family " << '\n';
        std::vector<std::vector<int>> A = {
            {4, 0},
            {0, 1}
        };
        std::vector<std::vector<int>> B = {
            {1, 0},
            {0, 4}
        };
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < A.front().size(); ++j) {
                pretty_print(A[i][j], B[i][j], check_nash(i,j,A,B), check_pareto(i,j,A,B));
            }
            std::cout << '\n';
        }
    }
    {
        std::cout << std::string(60, '=') << '\n';
        std::cout << "Prisoners" << '\n';
        std::vector<std::vector<int>> A = {
            {-5, 0},
            {-10, -1}
        };
        std::vector<std::vector<int>> B = {
            {-5, -10},
            {0, -1}
        };
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < A.front().size(); ++j) {
                pretty_print(A[i][j], B[i][j], check_nash(i,j,A,B), check_pareto(i,j,A,B));
            }
            std::cout << '\n';
        }
    }
    {
        std::cout << std::string(60, '=') << '\n';
        std::cout << "Mixed strategies" << '\n';
        std::vector<std::vector<double>> A = {
            {5, 7},
            {11, 6}
        };
        std::vector<std::vector<double>> B = {
            {8, 4},
            {7, 9}
        };
        for (size_t i = 0; i < A.size(); ++i) {
            for (size_t j = 0; j < A.front().size(); ++j) {
                pretty_print(static_cast<int>(A[i][j]), static_cast<int>(B[i][j]), check_nash(i,j,A,B), check_pareto(i,j,A,B));
            }
            std::cout << '\n';
        }
        solve_mixed(A,B);
    }
    return 0;
}
