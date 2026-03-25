#include <gtest/gtest.h>
#include "grove/pricing/black76.h"

using namespace grove::pricing;

TEST(Black76, CallPriceATM) {
    // ATM call: F=100, K=100, r=5%, T=1yr, sigma=20%
    auto result = black76_call(100.0, 100.0, 0.05, 1.0, 0.20);
    EXPECT_GT(result.price, 0.0);
    EXPECT_NEAR(result.price, 7.57, 0.1);  // approximate expected value
    EXPECT_GT(result.delta, 0.0);
    EXPECT_LT(result.delta, 1.0);
}

TEST(Black76, PutPriceATM) {
    auto result = black76_put(100.0, 100.0, 0.05, 1.0, 0.20);
    EXPECT_GT(result.price, 0.0);
    EXPECT_GT(result.delta, -1.0);
    EXPECT_LT(result.delta, 0.0);
}

TEST(Black76, PutCallParity) {
    double F = 75.0, K = 80.0, r = 0.03, T = 0.5, sigma = 0.25;
    auto call = black76_call(F, K, r, T, sigma);
    auto put = black76_put(F, K, r, T, sigma);
    double df = std::exp(-r * T);
    // C - P = df * (F - K)
    EXPECT_NEAR(call.price - put.price, df * (F - K), 1e-10);
}

TEST(Black76, ImpliedVol) {
    double F = 100.0, K = 100.0, r = 0.05, T = 1.0, sigma = 0.25;
    auto result = black76_call(F, K, r, T, sigma);
    double iv = black76_implied_vol(result.price, F, K, r, T, true);
    EXPECT_NEAR(iv, sigma, 1e-6);
}

TEST(Black76, ZeroTimeToExpiry) {
    auto call = black76_call(105.0, 100.0, 0.05, 0.0, 0.20);
    EXPECT_NEAR(call.price, 5.0, 1e-10);  // intrinsic only
    EXPECT_NEAR(call.delta, 1.0, 1e-10);  // deep ITM

    auto put = black76_put(95.0, 100.0, 0.05, 0.0, 0.20);
    EXPECT_NEAR(put.price, 5.0, 1e-10);
}
