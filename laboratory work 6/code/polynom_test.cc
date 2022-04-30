#include <gtest/gtest.h>
#include "metafuctions.hpp"

using namespace meta;

TEST(PowTest, Correctness) {
    int val;

    val = fastpow<10, 0>::value;
    EXPECT_EQ(val, 1);

    val = fastpow<5, 2>::value;
    EXPECT_EQ(val, 25);

    val = fastpow<-5, 2>::value;
    EXPECT_EQ(val, 25);

    val = fastpow<-5, 3>::value;
    EXPECT_EQ(val, -125);

    val = fastpow<100, 1>::value;
    EXPECT_EQ(val, 100);

    val = fastpow<9, 7>::value;
    EXPECT_EQ(val, 4782969);

    val = fastpow<4, 10>::value;
    EXPECT_EQ(val, 1048576);

    val = fastpow<2, 23>::value;
    EXPECT_EQ(val, 8388608);

    val = fastpow<-2, 23>::value;
    EXPECT_EQ(val, -8388608);

    val = fastpow<0, 0>::value;
    EXPECT_EQ(val, 1);
}

TEST(PolynomTest, Zeroed) {
    EXPECT_EQ(Polynomial<0>::get_y<0>::value, 0);
    EXPECT_EQ(Polynomial<0>::get_y<10>::value, 0);
    EXPECT_EQ(Polynomial<10>::get_y<0>::value, 10);
}

TEST(PolynomTest, ComputationWithPositiveNumbers) {
    int val;

    val = Polynomial<1, 2, 3, 4>::get_y<0>::value;
    EXPECT_EQ(val, 1);

    val = Polynomial<1, 2, 3, 4>::get_y<1>::value;
    EXPECT_EQ(val, 10);

    val = Polynomial<1, 2, 3, 4>::get_y<10>::value;
    EXPECT_EQ(val, 4321);

    val = Polynomial<0, 0, 1, 0>::get_y<5>::value;
    EXPECT_EQ(val, 25);

    val = Polynomial<1, 3, 6, 9>::get_y<0>::value;
    EXPECT_EQ(val, 1);

    val = Polynomial<0, 1, 1, 1, 1, 1, 1, 1, 1>::get_y<2>::value;
    EXPECT_EQ(val, 510);

    val = Polynomial<0, 0, 1, 0, 0, 1, 1>::get_y<2>::value;
    EXPECT_EQ(val, 100);

    val = Polynomial<32424252,747238275,82482489>::get_y<2>::value;
    EXPECT_EQ(val, 1856830758);
}

TEST(PolynomTest, ComputationWithNegativeNumbers) {
    int val;

    val = Polynomial<-1, -2, -3, -4>::get_y<0>::value;
    EXPECT_EQ(val, -1);

    val = Polynomial<-1, -2, -3, -4>::get_y<1>::value;
    EXPECT_EQ(val, -10);

    val = Polynomial<-1, -2, -3, -4>::get_y<20>::value;
    EXPECT_EQ(val, -33241);

    val = Polynomial<0, 0, 5, 6, 0, 3>::get_y<-4>::value;
    EXPECT_EQ(val, -3376);

    val = Polynomial<0, 0, 1, 0, 1, 0>::get_y<-1>::value;
    EXPECT_EQ(val, 2);

    val = Polynomial<-43535,5434,-2434>::get_y<47>::value;
    EXPECT_EQ(val, -5164843);
}

TEST(PolynomTest, Output) {
    std::string output;

    testing::internal::CaptureStdout();
    std::cout << Polynomial<5>();
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "P(X) = 5");

    testing::internal::CaptureStdout();
    std::cout << Polynomial<1, 2, 3, 4, 5>();
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "P(X) = 1+2·X^1+3·X^2+4·X^3+5·X^4");

    testing::internal::CaptureStdout();
    std::cout << Polynomial<-1, -1, -1, -1, -1, -1, -1, -1, -1>();
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "P(X) = -1-1·X^1-1·X^2-1·X^3-1·X^4-1·X^5-1·X^6-1·X^7-1·X^8");

    testing::internal::CaptureStdout();
    std::cout << Polynomial<0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,5,6,7>();
    output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "P(X) = 0+0·X^1+0·X^2+0·X^3+0·X^4+0·X^5+0·X^6+0·X^7+0·X^8+0·X^9+0·X^10+0·X^11+0·X^12+0·X^13+1·X^14+1·X^15+5·X^16+6·X^17+7·X^18");
}

TEST(DerivativeCoefficientTest, Correctness) {
    unsigned val;

    val = derivative_coefficient_counter<5, 1>::value;
    EXPECT_EQ(val, 5);

    val = derivative_coefficient_counter<5, 2>::value;
    EXPECT_EQ(val, 20);

    val = derivative_coefficient_counter<5, 0>::value;
    EXPECT_EQ(val, 1);

    val = derivative_coefficient_counter<5, 5>::value;
    EXPECT_EQ(val, 120);

    val = derivative_coefficient_counter<5, 6>::value;
    EXPECT_EQ(val, 0);
}

TEST(DerivativeTest, ComputingUpToFirstOrder) {
    unsigned val;

    val = Polynomial<1,2,3>::derivative<0>::value;
    EXPECT_EQ(val, 2);

    val = Polynomial<1,2,3>::derivative<1>::value;
    EXPECT_EQ(val, 8);

    val = Polynomial<2,2,2>::derivative<2>::value;
    EXPECT_EQ(val, 10);
}
