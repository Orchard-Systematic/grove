#include "grove/signals/kalman_filter.h"

namespace grove::signals {

KalmanFilter1D::KalmanFilter1D(double process_noise, double measurement_noise,
                               double initial_estimate, double initial_error)
    : q_(process_noise), r_(measurement_noise),
      x_(initial_estimate), p_(initial_error), k_(0.0) {}

double KalmanFilter1D::update(double measurement) {
    // Predict step (for 1D random walk: predicted state = current state)
    double p_pred = p_ + q_;

    // Update step
    k_ = p_pred / (p_pred + r_);
    x_ = x_ + k_ * (measurement - x_);
    p_ = (1.0 - k_) * p_pred;

    return x_;
}

void KalmanFilter1D::reset(double initial_estimate, double initial_error) {
    x_ = initial_estimate;
    p_ = initial_error;
    k_ = 0.0;
}

std::vector<double> KalmanFilter1D::filter_series(const std::vector<double>& values,
                                                   double process_noise,
                                                   double measurement_noise) {
    if (values.empty()) return {};

    KalmanFilter1D kf(process_noise, measurement_noise, values[0]);
    std::vector<double> result;
    result.reserve(values.size());

    for (double v : values) {
        result.push_back(kf.update(v));
    }
    return result;
}

} // namespace grove::signals
