#include "grove/curves/cubic_spline.h"

#include <algorithm>
#include <stdexcept>

namespace grove::curves {

CubicSpline::CubicSpline(const std::vector<double>& x, const std::vector<double>& y)
    : x_(x), y_(y) {
    if (x_.size() != y_.size()) {
        throw std::invalid_argument("x and y must have the same length");
    }
    if (x_.size() < 2) {
        throw std::invalid_argument("at least 2 points are required");
    }
    compute_coefficients();
}

void CubicSpline::compute_coefficients() {
    size_t n = x_.size() - 1;  // number of intervals

    // h[i] = x[i+1] - x[i]
    std::vector<double> h(n);
    for (size_t i = 0; i < n; ++i) {
        h[i] = x_[i + 1] - x_[i];
        if (h[i] <= 0.0) {
            throw std::invalid_argument("x values must be strictly increasing");
        }
    }

    // Solve tridiagonal system for second derivatives (natural spline: S''(0)=S''(n)=0)
    a_.resize(n + 1, 0.0);
    b_.resize(n);
    c_.resize(n + 1, 0.0);
    d_.resize(n);

    if (n > 1) {
        // Set up the tridiagonal system
        std::vector<double> alpha(n);
        for (size_t i = 1; i < n; ++i) {
            alpha[i] = 3.0 / h[i] * (y_[i + 1] - y_[i])
                     - 3.0 / h[i - 1] * (y_[i] - y_[i - 1]);
        }

        std::vector<double> l(n + 1), mu(n + 1), z(n + 1);
        l[0] = 1.0; mu[0] = 0.0; z[0] = 0.0;

        for (size_t i = 1; i < n; ++i) {
            l[i] = 2.0 * (x_[i + 1] - x_[i - 1]) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }

        l[n] = 1.0; z[n] = 0.0; c_[n] = 0.0;

        for (int j = static_cast<int>(n) - 1; j >= 0; --j) {
            size_t ju = static_cast<size_t>(j);
            c_[ju] = z[ju] - mu[ju] * c_[ju + 1];
            b_[ju] = (y_[ju + 1] - y_[ju]) / h[ju]
                   - h[ju] * (c_[ju + 1] + 2.0 * c_[ju]) / 3.0;
            d_[ju] = (c_[ju + 1] - c_[ju]) / (3.0 * h[ju]);
            a_[ju] = y_[ju];
        }
    } else {
        // Only 2 points: linear interpolation
        a_[0] = y_[0];
        b_[0] = (y_[1] - y_[0]) / h[0];
        c_[0] = 0.0;
        d_[0] = 0.0;
    }
}

size_t CubicSpline::find_segment(double x) const {
    // Binary search for the interval
    if (x <= x_.front()) return 0;
    if (x >= x_.back()) return x_.size() - 2;

    auto it = std::upper_bound(x_.begin(), x_.end(), x);
    return static_cast<size_t>(std::distance(x_.begin(), it)) - 1;
}

double CubicSpline::evaluate(double x) const {
    size_t i = find_segment(x);
    double dx = x - x_[i];
    return a_[i] + b_[i] * dx + c_[i] * dx * dx + d_[i] * dx * dx * dx;
}

double CubicSpline::derivative(double x) const {
    size_t i = find_segment(x);
    double dx = x - x_[i];
    return b_[i] + 2.0 * c_[i] * dx + 3.0 * d_[i] * dx * dx;
}

double CubicSpline::second_derivative(double x) const {
    size_t i = find_segment(x);
    double dx = x - x_[i];
    return 2.0 * c_[i] + 6.0 * d_[i] * dx;
}

} // namespace grove::curves
