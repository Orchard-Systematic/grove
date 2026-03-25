#include <gtest/gtest.h>
#include "grove/signals/moving_average.h"
#include <vector>

using namespace grove::signals;

TEST(MovingAverage, SMAConstant) {
    std::vector<double> prices(100, 50.0);
    auto result = sma(prices, 20);
    ASSERT_EQ(result.size(), 100u);
    // After warmup, SMA of constant series = constant
    EXPECT_NEAR(result.back(), 50.0, 1e-10);
}

TEST(MovingAverage, EMAConverges) {
    std::vector<double> prices(200, 0.0);
    for (size_t i = 100; i < 200; ++i) prices[i] = 100.0;
    auto result = ema(prices, 10);
    // EMA should converge toward 100 after the step
    EXPECT_GT(result.back(), 99.0);
}

TEST(MovingAverage, CrossoverDetection) {
    // Rising prices should produce a bullish crossover
    std::vector<double> prices;
    for (int i = 0; i < 100; ++i) prices.push_back(50.0);
    for (int i = 0; i < 100; ++i) prices.push_back(50.0 + i * 0.5);

    auto signals = ema_crossover(prices, 5, 20);
    bool found_bullish = false;
    for (auto s : signals) {
        if (s == 1) { found_bullish = true; break; }
    }
    EXPECT_TRUE(found_bullish);
}
