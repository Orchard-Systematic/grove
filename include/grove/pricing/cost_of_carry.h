#pragma once

namespace grove::pricing {

/// Cost-of-carry model for commodity futures.
/// F = S * exp((r - y + u) * T)
///   S = spot price
///   r = risk-free rate
///   y = convenience yield
///   u = storage cost rate

/// Calculate theoretical futures price from spot.
double futures_fair_value(double spot, double rate, double convenience_yield,
                          double storage_cost, double time_to_expiry);

/// Implied convenience yield from observed futures and spot.
double implied_convenience_yield(double spot, double futures_price,
                                 double rate, double storage_cost,
                                 double time_to_expiry);

/// Basis (futures - spot), normalised by spot if requested.
double basis(double spot, double futures_price, bool normalise = false);

/// Annualised basis (carry return).
double annualised_basis(double spot, double futures_price, double days_to_expiry);

} // namespace grove::pricing
