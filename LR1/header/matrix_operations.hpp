#pragma once
#include <exception>
#include <functional>
#include <numeric>
#include <vector>
#include <iostream>

template<typename T>
void print_vector(const std::vector<T>& v) {
    std::cout << "( ";
    for (const auto& el : v) {
        std::cout << el << " ";
    }
    std::cout << ")" << std::endl;
}
template<typename T>
void print_matrix(const std::vector<std::vector<T>>& m) {
    for (const auto& row : m) {
        print_vector(row);
    }
}

template<typename T>
T vector_vector_mul(const std::vector<T>& v1, const std::vector<T>& v2) {
    if (v1.size() != v2.size()) throw std::logic_error{"Vector dimensions are different!"};
    return std::inner_product(v1.begin(), v1.end(), v2.begin(), T());
}

template<typename T>
std::vector<T> number_vector_mul(const T& num, const std::vector<T>& vec) {
    std::vector<T> res;
    res.resize(vec.size());
    std::transform(vec.begin(),vec.end(), res.begin(), [&num](T el) { return el*num; });
    return res;
}
template<typename T>
std::vector<T> vector_number_div(const T& num, const std::vector<T>& vec) {
    auto inv_num = num;
    return number_vector_mul(inv_num.inverse(), vec);
}

template<typename T>
auto get_transposed_matrix(const std::vector<std::vector<T>>& matrix) {
    std::vector<std::vector<T> > tmp(matrix[0].size(), std::vector<T>(matrix.size()));
    auto m_row_size = matrix[0].size();
    auto m_col_size = matrix.size();
    for (std::vector<int>::size_type i = 0; i < m_row_size; i++) 
        for (std::vector<int>::size_type j = 0; j < m_col_size; j++) {
            tmp[i][j] = matrix[j][i];
        }
    return tmp;
}

template<typename T>
std::vector<T> vector_matrix_mul(const std::vector<T>& vec, const std::vector<std::vector<T>>& matrix) {
    if (vec.size() != matrix.size()) throw std::logic_error{"Vector and matrix row dimensions are different!"};
    std::vector<T> result;
    result.reserve(vec.size());

    auto matrix_tmp = get_transposed_matrix(matrix);
    for (const auto& row : matrix_tmp) {
        result.push_back(vector_vector_mul(row, vec));
    }
    return std::move(result);
}

template<typename T>
std::vector<T> matrix_vector_mul(const std::vector<std::vector<T>>& matrix, const std::vector<T>& vec) {
    if (vec.size() != matrix[0].size()) throw std::logic_error{"Vector and matrix row dimensions are different!"};
    std::vector<T> result;
    result.reserve(vec.size());

    for (const auto& row : matrix) {
        result.push_back(vector_vector_mul(row, vec));
    }
    return std::move(result);
}
template<typename T>
auto get_matrix_determinant(const std::vector<std::vector<T>>& matrix) {
    if (matrix.size() != matrix[0].size()) {
        throw std::logic_error("Matrix is not quadratic");
    }
    size_t dimension = matrix.size();

    if (dimension == 0) {
        return T(1);
    }
    if (dimension == 1) {
        return matrix[0][0];
    }
    if (dimension == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    T result = 0;
    int sign = 1;
    for (int i = 0; i < dimension; i++) {
        std::vector<std::vector<T>> sub_matrix {dimension - 1, std::vector<T> (dimension - 1)};
        for(int m = 1; m < dimension; m++) {
            int z = 0;
            for(int n = 0; n < dimension; n++) {
                if(n != i) {
                    sub_matrix[m-1][z] = matrix[m][n];
                    z++;
                }
            }
        }
        result = result + sign * matrix[0][i] * get_matrix_determinant(sub_matrix);
        sign = -sign;
    }
    return result;
}

template<typename T>
auto get_cofactor(const std::vector<std::vector<T>> matrix) {
    if(matrix.size() != matrix[0].size()) {
        throw std::runtime_error("Matrix is not quadratic");
    }

    std::vector<std::vector<T>> solution {matrix.size(), std::vector<T> (matrix.size())};
    std::vector<std::vector<T>> sub_matrix {matrix.size() - 1, std::vector<T> (matrix.size() - 1)};

    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[0].size(); j++) {

            int p = 0;
            for (size_t x = 0; x < matrix.size(); x++) {
                if (x == i) {
                    continue;
                }
                int q = 0;

                for (size_t y = 0; y < matrix.size(); y++) {
                    if(y == j) {
                        continue;
                    }

                    sub_matrix[p][q] = matrix[x][y];
                    q++;
                }
                p++;
            }
            solution[i][j] = std::pow(-1, i + j) * get_matrix_determinant(sub_matrix);
        }
    }
    return solution;
}

template<typename T>
auto get_inversed_matrix(std::vector<std::vector<T>> matrix) {
    T det = get_matrix_determinant(matrix);
    if(det == T(0)) {
        throw std::runtime_error("Determinant is 0");
    } 
    std::vector<std::vector<T>> solution(matrix.size(), std::vector<T> (matrix.size()));

    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix.size(); j++) {
            solution[i][j] = matrix[i][j]; 
        }
    }

    auto cofactor = get_cofactor(solution);
    solution = get_transposed_matrix(get_cofactor(solution));

    for (auto& row : solution) {
        row = vector_number_div(det, row);
    }
    return solution;
}
