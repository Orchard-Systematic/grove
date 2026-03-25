#include <gtest/gtest.h>
#include "grove/pricing/calendar_spread.h"

using namespace grove::pricing;

TEST(CalendarSpread, Backwardation) {
    auto result = analyse_calendar_spread(75.0, 72.0, 30.0);
    EXPECT_TRUE(result.is_backwardation);
    EXPECT_FALSE(result.is_contango);
    EXPECT_NEAR(result.spread_value, 3.0, 1e-10);
}

TEST(CalendarSpread, Contango) {
    auto result = analyse_calendar_spread(72.0, 75.0, 30.0);
    EXPECT_TRUE(result.is_contango);
    EXPECT_FALSE(result.is_backwardation);
}

TEST(CalendarSpread, RollYield) {
    double ry = roll_yield(75.0, 72.0, 30.0);
    double expected = (3.0 / 72.0) * (365.0 / 30.0);
    EXPECT_NEAR(ry, expected, 1e-10);
}
