#include <matrix_operations.hpp>
#include <quotient.hpp>
#include <gtest/gtest.h>

TEST(MATRIX_OPS_TEST, vector_vector_mul) {
    std::vector<Q> vec1 = {
        {5,48}, {1,8}
    };
    std::vector<Q> vec2 = {
        {1}, {1}
    };
    //Q ans{11,48};
    EXPECT_EQ(vector_vector_mul<Q>(vec1,vec2), "11/48");
}
TEST(MATRIX_OPS_TEST, matrix_transpose) {
    std::vector<std::vector<Q>> matrix = {
            {{-1,16},{1,8}},
            {{1,6},{}}
    };
    std::vector<std::vector<Q>> expected_matrix = {
            {{-1,16},{1,6}},
            {{1,8},{}}
    };
    auto tmp = get_transposed_matrix<Q>(matrix);
    EXPECT_EQ(tmp, expected_matrix);
}
TEST(MATRIX_OPS_TEST, vector_matrix_mul) {
    std::vector<Q> vec = {
        {1}, {1}
    };
    std::vector<std::vector<Q>> matrix = {
        {{-1,16},{1,8}},
        {{1,6},{}}
    };
    std::vector<Q> ans = {
        {5,48}, {1,8}
    };
    auto product = vector_matrix_mul<Q>(vec, matrix);
    EXPECT_EQ(product, ans);
}
TEST(MATRIX_OPS_TEST, matrix_vector_mul) {
    std::vector<std::vector<Q>> matrix = {
        {{-1,16},{1,8}},
        {{1,6},{}}
    };
    std::vector<Q> vec = {
            {1}, {1}
    };
    std::vector<Q> ans = {
        {1,16}, {1,6}
    };
    EXPECT_EQ(matrix_vector_mul<Q>(matrix, vec), ans);
}
TEST(MATRIX_OPS_TEST, num_vector_mul) {
    std::vector<Q> vec = { {1,16}, {1,6} };
    Q num{48, 11};
    std::vector<Q> ans = {
            {3,11}, {8,11}
    };
    EXPECT_EQ(number_vector_mul(num, vec), ans);
}
TEST(MATRIX_OPS_TEST, num_vector_div) {
    std::vector<Q> vec = { {1,16}, {1,6} };
    Q num = {11, 48};
    std::vector<Q> ans = {
            {3,11}, {8,11}
    };
    EXPECT_EQ(vector_number_div(num, vec), ans);
}
TEST(MATRIX_OPS_TEST, inverse_matrix) {
    std::vector<std::vector<Q>> matrix = {
        {{0},{6}},
        {{8},{3}}
    };
    std::vector<std::vector<Q>> ans = {
        {{-1,16},{1,8}},
        {{1,6},{0}}
    };
    auto inversed = get_inversed_matrix(matrix);
    EXPECT_EQ(inversed, ans);
}
