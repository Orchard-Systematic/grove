#include <gtest/gtest.h>
#include "grove/signals/kalman_filter.h"
#include <cmath>

using namespace grove::signals;

TEST(KalmanFilter, ConstantSignal) {
    KalmanFilter1D kf(0.001, 1.0, 0.0);
    for (int i = 0; i < 100; ++i) {
        kf.update(50.0);
    }
    EXPECT_NEAR(kf.estimate(), 50.0, 0.1);
}

TEST(KalmanFilter, TracksStep) {
    KalmanFilter1D kf(0.1, 1.0, 0.0);
    for (int i = 0; i < 50; ++i) kf.update(0.0);
    for (int i = 0; i < 50; ++i) kf.update(100.0);
    EXPECT_GT(kf.estimate(), 90.0);
}

TEST(KalmanFilter, BatchFilter) {
    std::vector<double> values = {1.0, 2.0, 3.0, 4.0, 5.0};
    auto result = KalmanFilter1D::filter_series(values, 0.1, 1.0);
    ASSERT_EQ(result.size(), 5u);
    // Filtered values should be smooth and increasing
    for (size_t i = 1; i < result.size(); ++i) {
        EXPECT_GT(result[i], result[i - 1]);
    }
}

TEST(KalmanFilter, Reset) {
    KalmanFilter1D kf(0.1, 1.0, 100.0);
    kf.update(50.0);
    kf.reset(0.0);
    EXPECT_NEAR(kf.estimate(), 0.0, 1e-10);
}
