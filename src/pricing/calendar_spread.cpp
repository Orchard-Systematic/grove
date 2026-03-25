#include "grove/pricing/calendar_spread.h"

#include <cmath>

namespace grove::pricing {

CalendarSpreadResult analyse_calendar_spread(double front_price,
                                             double back_price,
                                             double days_between) {
    CalendarSpreadResult result;
    result.spread_value = front_price - back_price;
    result.spread_ratio = (back_price != 0.0) ? front_price / back_price : 0.0;
    result.roll_yield_annualised = roll_yield(front_price, back_price, days_between);
    result.is_contango = front_price < back_price;
    result.is_backwardation = front_price > back_price;
    return result;
}

double roll_yield(double front_price, double back_price, double days_between) {
    if (back_price <= 0.0 || days_between <= 0.0) return 0.0;
    double period_return = (front_price - back_price) / back_price;
    return period_return * (365.0 / days_between);
}

} // namespace grove::pricing
