#include <gtest/gtest.h>
#include <quotient.hpp>

// use fixtures!!!!!
TEST(QUOTIENT_TEST, construction_default) {
    Q q1;
    EXPECT_EQ(q1.get_m(), 0);
    EXPECT_EQ(q1.get_n(), 1);
}
TEST(QUOTIENT_TEST, less_compare) {
    Q q1 = {1,2};
    Q q2 = {1,4};
    EXPECT_TRUE(q1 > q2);
    Q q3 = {6,4};
    Q q4 = {6,3};
    EXPECT_TRUE(q3 < q4);
}
TEST(QUOTIENT_TEST, construction_from_int) {
    Q q2{12};
    EXPECT_EQ(q2.get_m(), 12);
    EXPECT_EQ(q2.get_n(), 1);
    Q q3 = 8;
    EXPECT_EQ(q3, "8");
}
TEST(QUOTIENT_TEST, construction_from_int_implicit) {
    Q q2 = 12;
    EXPECT_EQ(q2.get_m(), 12);
    EXPECT_EQ(q2.get_n(), 1);
}
TEST(QUOTIENT_TEST, construction_ratio) {
    Q q3{1, 2};
    EXPECT_EQ(q3.get_m(), 1);
    EXPECT_EQ(q3.get_n(), 2);
}
TEST(QUOTIENT_TEST, std_repr_test_empty) {
    Q q1;
    EXPECT_EQ(q1, "0");
}
TEST(QUOTIENT_TEST, std_repr_test_int) {
    Q q2{12};
    EXPECT_EQ(q2, "12");
}
TEST(QUOTIENT_TEST, std_repr_test_ratio) {
    Q q3{1, 2};
    EXPECT_EQ(q3, "1/2");
}
TEST(QUOTIENT_TEST, std_repr_test_one) {
    Q q4{2, 2};
    EXPECT_EQ(q4, "1");
}
TEST(QUOTIENT_TEST, std_repr_test_negative) {
    Q q5{-1, 2};
    EXPECT_EQ(q5, std::string{"-1/2"});
}
TEST(QUOTIENT_TEST, adding_rations) {
    Q q1{1, 2};
    Q q2{3, 4};
    auto q3 = q1 + q2;
    EXPECT_EQ(q3, "5/4");
    auto q4 = Q{1,2} + Q{1,2};
    EXPECT_EQ(q4, "1");
}
TEST(QUOTIENT_TEST, adding_int) {
    Q q1{1, 2};
    auto q4 = q1 + 4;
    EXPECT_EQ(q4, "9/2");
}
TEST(QUOTIENT_TEST, adding_equal_plus_int) {
    Q q1{1, 2};
    q1 += 4;
    EXPECT_EQ(q1, "9/2");
}
TEST(QUOTIENT_TEST, adding_equal_plus_ratio) {
    Q q1{1, 2};
    Q q2{3, 4};
    q1 += q2;
    EXPECT_EQ(q1, "5/4");
}
TEST(QUOTIENT_TEST, mult_ratio) {
    Q q1{5, 2};
    Q q2{3, 4};
    auto q3 = q1 * q2;
    EXPECT_EQ(q3, "15/8");
    Q q4{"-1/48"};
    Q q5{3};
    auto q6 = q4 * q5;
    EXPECT_EQ(q6, "-1/16");

}
TEST(QUOTIENT_TEST, mult_int) {
    Q q1{5, 2};
    auto q4 = q1 * 4;
    EXPECT_EQ(q4, "10");
}
TEST(QUOTIENT_TEST, normalize_sum) {
    Q q1{5, 2};
    Q q2{2, 4};
    auto q4 = q1 + q2;
    EXPECT_EQ(q4, "3");
}
TEST(QUOTIENT_TEST, normalize_mult) {
    Q q1{5, 2};
    Q q2{2, 4};
    auto q3 = q1 * q2;
    EXPECT_EQ(q3, "5/4");
}
