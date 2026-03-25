#pragma once

#include <vector>

namespace grove::signals {

/// Simple 1D Kalman filter for smoothing price/signal series.
/// Models: x(k) = A*x(k-1) + w,  z(k) = H*x(k) + v
/// For a 1D price filter: A=1, H=1, tuned by process_noise (Q) and
/// measurement_noise (R).

class KalmanFilter1D {
public:
    /// @param process_noise      Q — how much we expect the true state to change
    /// @param measurement_noise  R — how noisy our observations are
    /// @param initial_estimate   Starting value
    /// @param initial_error      Starting error covariance
    KalmanFilter1D(double process_noise, double measurement_noise,
                   double initial_estimate = 0.0, double initial_error = 1.0);

    /// Process a new observation and return the filtered estimate.
    double update(double measurement);

    /// Get current state estimate.
    double estimate() const { return x_; }

    /// Get current error covariance.
    double error_covariance() const { return p_; }

    /// Get the Kalman gain from the last update.
    double gain() const { return k_; }

    /// Reset the filter state.
    void reset(double initial_estimate = 0.0, double initial_error = 1.0);

    /// Batch-filter an entire series.
    static std::vector<double> filter_series(const std::vector<double>& values,
                                              double process_noise,
                                              double measurement_noise);

private:
    double q_;  // process noise
    double r_;  // measurement noise
    double x_;  // state estimate
    double p_;  // error covariance
    double k_;  // Kalman gain
};

} // namespace grove::signals
