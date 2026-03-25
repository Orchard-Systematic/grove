#pragma once

#include <vector>

namespace grove::curves {

/// Natural cubic spline interpolation.
/// Fits a smooth curve through commodity forward curve points.

class CubicSpline {
public:
    /// Construct spline from knot points.
    /// @param x  x-coordinates (must be sorted ascending, at least 2 points)
    /// @param y  y-coordinates (same length as x)
    CubicSpline(const std::vector<double>& x, const std::vector<double>& y);

    /// Evaluate the spline at a given point.
    /// Extrapolates linearly outside the knot range.
    double evaluate(double x) const;

    /// Evaluate the first derivative at a given point.
    double derivative(double x) const;

    /// Evaluate the second derivative at a given point.
    double second_derivative(double x) const;

    /// Number of knot points.
    size_t size() const { return x_.size(); }

private:
    std::vector<double> x_, y_;
    std::vector<double> a_, b_, c_, d_;  // spline coefficients

    void compute_coefficients();
    size_t find_segment(double x) const;
};

} // namespace grove::curves
