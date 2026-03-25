#include <gtest/gtest.h>
#include "grove/curves/bootstrap.h"

using namespace grove::curves;

TEST(Bootstrap, SortedOutput) {
    std::vector<double> tenors = {1.0, 0.25, 0.5, 2.0};
    std::vector<double> prices = {72.0, 75.0, 74.0, 70.0};
    auto curve = bootstrap_forward_curve(tenors, prices);

    ASSERT_EQ(curve.size(), 4u);
    EXPECT_LT(curve[0].tenor, curve[1].tenor);
    EXPECT_LT(curve[1].tenor, curve[2].tenor);
}

TEST(Bootstrap, Interpolation) {
    std::vector<double> tenors = {0.25, 0.5, 1.0};
    std::vector<double> prices = {75.0, 74.0, 72.0};
    auto curve = bootstrap_forward_curve(tenors, prices);

    double val = interpolate_curve(curve, 0.375);
    EXPECT_GT(val, 74.0);
    EXPECT_LT(val, 75.0);
}

TEST(Bootstrap, FlatExtrapolation) {
    std::vector<double> tenors = {0.25, 1.0};
    std::vector<double> prices = {75.0, 72.0};
    auto curve = bootstrap_forward_curve(tenors, prices);

    EXPECT_NEAR(interpolate_curve(curve, 0.0), 75.0, 1e-10);
    EXPECT_NEAR(interpolate_curve(curve, 5.0), 72.0, 1e-10);
}
