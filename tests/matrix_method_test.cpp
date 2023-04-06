#include <memory>

#include <gtest/gtest.h>
#include <matrix_method.hpp>

class MatrixMethodTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::vector<std::vector<double>> game_matrix = {
            {2, 1, 3},
            {3, 0, 1},
            {1, 2, 1}
        };
        std::vector<std::vector<Q>> game_matrix_q = {
            {2, 1, 3},
            {3, 0, 1},
            {1, 2, 1}
        };
        md_solver_ptr = std::make_unique<MatrixMethodSolver<double>>(game_matrix);
        mq_solver_ptr = std::make_unique<MatrixMethodSolver<Q>>(game_matrix_q);
    }
    std::unique_ptr<MatrixMethodSolver<double>> md_solver_ptr;
    std::unique_ptr<MatrixMethodSolver<Q>> mq_solver_ptr;
};


TEST_F(MatrixMethodTest, GameMatrixDoubleValuesSolution) {
    md_solver_ptr->init();
    md_solver_ptr->solve();

    auto [gc, player_A_solution, player_B_solution] = md_solver_ptr->get_solution(0);
    EXPECT_EQ(gc, 1.5);
    std::vector<double> expected_plA = {0.5, 0.5, 0};
    std::vector<double> expected_plB = {0.25, 0.125, 0.625};
    EXPECT_EQ(player_A_solution.size(), expected_plA.size());
    EXPECT_EQ(player_B_solution.size(), expected_plB.size());
    for (size_t i = 0; i < expected_plA.size(); ++i) {
        EXPECT_DOUBLE_EQ(player_A_solution[i], expected_plA[i]);
    }
    for (size_t i = 0; i < expected_plB.size(); ++i) {
        EXPECT_DOUBLE_EQ(player_B_solution[i], expected_plB[i]);
    }
}

TEST_F(MatrixMethodTest, GameMatrixQuotientValuesSolution) {
    mq_solver_ptr->init();
    mq_solver_ptr->solve();

    auto [gc, player_A_solution, player_B_solution] = mq_solver_ptr->get_solution(0);
    EXPECT_EQ(gc, "3/2");
    std::vector<Q> expected_plA = {{"1/2"}, {"1/2"}, {"0"}};
    std::vector<Q> expected_plB = {{"1/4"}, {"1/8"}, {"5/8"}};
    EXPECT_EQ(player_A_solution.size(), expected_plA.size());
    EXPECT_EQ(player_B_solution.size(), expected_plB.size());
    for (size_t i = 0; i < expected_plA.size(); ++i) {
        EXPECT_EQ(player_A_solution[i], expected_plA[i]);
    }
    for (size_t i = 0; i < expected_plB.size(); ++i) {
        EXPECT_EQ(player_B_solution[i], expected_plB[i]);
    }
}
