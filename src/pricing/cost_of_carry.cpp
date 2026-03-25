#include "grove/pricing/cost_of_carry.h"

#include <cmath>

namespace grove::pricing {

double futures_fair_value(double spot, double rate, double convenience_yield,
                          double storage_cost, double time_to_expiry) {
    return spot * std::exp((rate - convenience_yield + storage_cost) * time_to_expiry);
}

double implied_convenience_yield(double spot, double futures_price,
                                 double rate, double storage_cost,
                                 double time_to_expiry) {
    if (time_to_expiry <= 0.0 || spot <= 0.0) return 0.0;
    return rate + storage_cost - std::log(futures_price / spot) / time_to_expiry;
}

double basis(double spot, double futures_price, bool normalise) {
    double raw = futures_price - spot;
    if (normalise && spot != 0.0) {
        return raw / spot;
    }
    return raw;
}

double annualised_basis(double spot, double futures_price, double days_to_expiry) {
    if (days_to_expiry <= 0.0 || spot <= 0.0) return 0.0;
    double raw_basis = (futures_price - spot) / spot;
    return raw_basis * (365.0 / days_to_expiry);
}

} // namespace grove::pricing
