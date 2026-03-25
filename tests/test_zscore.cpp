#include <gtest/gtest.h>
#include "grove/signals/zscore.h"
#include <vector>

using namespace grove::signals;

TEST(ZScore, ConstantSeries) {
    std::vector<double> values(50, 100.0);
    auto result = rolling_zscore(values, 20);
    // Constant series has zero z-score (std = 0)
    for (auto z : result.zscore) {
        EXPECT_NEAR(z, 0.0, 1e-10);
    }
}

TEST(ZScore, Outlier) {
    std::vector<double> values(50, 100.0);
    values.back() = 110.0;  // spike
    auto result = rolling_zscore(values, 20);
    // Last z-score should be large and positive
    EXPECT_GT(result.zscore.back(), 3.0);
}

TEST(ZScore, SingleFunction) {
    EXPECT_NEAR(zscore_single(110.0, 100.0, 5.0), 2.0, 1e-10);
    EXPECT_NEAR(zscore_single(100.0, 100.0, 5.0), 0.0, 1e-10);
}
