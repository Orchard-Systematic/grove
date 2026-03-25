#pragma once

#include <vector>

namespace grove::curves {

/// Bootstrap a discount/forward curve from a set of observed futures prices.
/// Uses piecewise linear interpolation between observed tenors.

struct CurvePoint {
    double tenor;   // time in years
    double value;   // price or rate at this tenor
};

/// Bootstrap a forward curve from observed prices and tenors.
/// @param tenors  Time to expiry for each observed contract (years)
/// @param prices  Observed futures prices (must be same length as tenors)
/// @return        Bootstrapped curve points (sorted by tenor)
std::vector<CurvePoint> bootstrap_forward_curve(
    const std::vector<double>& tenors,
    const std::vector<double>& prices);

/// Interpolate a value from a bootstrapped curve at a given tenor.
/// Uses linear interpolation between surrounding points.
double interpolate_curve(const std::vector<CurvePoint>& curve, double tenor);

} // namespace grove::curves
