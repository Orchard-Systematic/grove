#pragma once

namespace grove::pricing {

/// Calendar spread analytics for commodity futures.

struct CalendarSpreadResult {
    double spread_value;           // front - back
    double spread_ratio;           // front / back
    double roll_yield_annualised;  // annualised return from roll
    bool is_contango;              // true if front < back
    bool is_backwardation;         // true if front > back
};

/// Analyse a calendar spread between two contract months.
/// @param front_price  Price of the near-month contract
/// @param back_price   Price of the deferred contract
/// @param days_between Days between the two contract expiries
CalendarSpreadResult analyse_calendar_spread(double front_price,
                                             double back_price,
                                             double days_between);

/// Calculate the roll yield between two points on the forward curve.
double roll_yield(double front_price, double back_price, double days_between);

} // namespace grove::pricing
