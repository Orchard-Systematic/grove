#include "grove/curves/bootstrap.h"

#include <algorithm>
#include <stdexcept>

namespace grove::curves {

std::vector<CurvePoint> bootstrap_forward_curve(
    const std::vector<double>& tenors,
    const std::vector<double>& prices) {

    if (tenors.size() != prices.size()) {
        throw std::invalid_argument("tenors and prices must have the same length");
    }
    if (tenors.empty()) {
        throw std::invalid_argument("at least one point is required");
    }

    // Build curve points and sort by tenor
    std::vector<CurvePoint> curve;
    curve.reserve(tenors.size());
    for (size_t i = 0; i < tenors.size(); ++i) {
        curve.push_back({tenors[i], prices[i]});
    }

    std::sort(curve.begin(), curve.end(),
              [](const CurvePoint& a, const CurvePoint& b) {
                  return a.tenor < b.tenor;
              });

    return curve;
}

double interpolate_curve(const std::vector<CurvePoint>& curve, double tenor) {
    if (curve.empty()) {
        throw std::invalid_argument("curve is empty");
    }
    if (curve.size() == 1) {
        return curve[0].value;
    }

    // Clamp to curve boundaries (flat extrapolation)
    if (tenor <= curve.front().tenor) return curve.front().value;
    if (tenor >= curve.back().tenor) return curve.back().value;

    // Linear interpolation between surrounding points
    for (size_t i = 1; i < curve.size(); ++i) {
        if (tenor <= curve[i].tenor) {
            double t = (tenor - curve[i - 1].tenor) /
                       (curve[i].tenor - curve[i - 1].tenor);
            return curve[i - 1].value + t * (curve[i].value - curve[i - 1].value);
        }
    }

    return curve.back().value;
}

} // namespace grove::curves
