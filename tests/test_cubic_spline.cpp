#include <gtest/gtest.h>
#include "grove/curves/cubic_spline.h"
#include <cmath>

using namespace grove::curves;

TEST(CubicSpline, LinearData) {
    // Spline through linear data should interpolate exactly
    std::vector<double> x = {0, 1, 2, 3, 4};
    std::vector<double> y = {0, 2, 4, 6, 8};  // y = 2x
    CubicSpline spline(x, y);

    EXPECT_NEAR(spline.evaluate(0.5), 1.0, 1e-10);
    EXPECT_NEAR(spline.evaluate(2.5), 5.0, 1e-10);
    EXPECT_NEAR(spline.derivative(1.0), 2.0, 1e-10);
}

TEST(CubicSpline, QuadraticData) {
    std::vector<double> x = {0, 1, 2, 3, 4};
    std::vector<double> y = {0, 1, 4, 9, 16};  // y = x^2
    CubicSpline spline(x, y);

    // Should be close to x^2 at midpoints
    EXPECT_NEAR(spline.evaluate(1.5), 2.25, 0.1);
    EXPECT_NEAR(spline.evaluate(2.5), 6.25, 0.1);
}

TEST(CubicSpline, KnotInterpolation) {
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0};
    std::vector<double> y = {10.0, 20.0, 15.0, 25.0};
    CubicSpline spline(x, y);

    // Must pass through knot points exactly
    for (size_t i = 0; i < x.size(); ++i) {
        EXPECT_NEAR(spline.evaluate(x[i]), y[i], 1e-10);
    }
}

TEST(CubicSpline, TooFewPoints) {
    std::vector<double> x = {1.0};
    std::vector<double> y = {5.0};
    EXPECT_THROW(CubicSpline(x, y), std::invalid_argument);
}
