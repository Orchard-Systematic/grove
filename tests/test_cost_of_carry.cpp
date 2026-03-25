#include <gtest/gtest.h>
#include "grove/pricing/cost_of_carry.h"

using namespace grove::pricing;

TEST(CostOfCarry, FairValueNoYield) {
    // No convenience yield, no storage: F = S * exp(r * T)
    double fv = futures_fair_value(100.0, 0.05, 0.0, 0.0, 1.0);
    EXPECT_NEAR(fv, 100.0 * std::exp(0.05), 1e-10);
}

TEST(CostOfCarry, FairValueWithConvenienceYield) {
    double fv = futures_fair_value(100.0, 0.05, 0.03, 0.01, 1.0);
    EXPECT_NEAR(fv, 100.0 * std::exp(0.05 - 0.03 + 0.01), 1e-10);
}

TEST(CostOfCarry, ImpliedConvenienceYield) {
    double spot = 100.0, rate = 0.05, storage = 0.01, T = 0.5;
    double fv = futures_fair_value(spot, rate, 0.02, storage, T);
    double icy = implied_convenience_yield(spot, fv, rate, storage, T);
    EXPECT_NEAR(icy, 0.02, 1e-10);
}

TEST(CostOfCarry, Basis) {
    EXPECT_NEAR(basis(100.0, 105.0), 5.0, 1e-10);
    EXPECT_NEAR(basis(100.0, 105.0, true), 0.05, 1e-10);
}

TEST(CostOfCarry, AnnualisedBasis) {
    double ab = annualised_basis(100.0, 102.0, 90.0);
    double expected = (2.0 / 100.0) * (365.0 / 90.0);
    EXPECT_NEAR(ab, expected, 1e-10);
}
