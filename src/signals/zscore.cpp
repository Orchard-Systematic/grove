#include "grove/signals/zscore.h"

#include <cmath>
#include <stdexcept>

namespace grove::signals {

ZScoreResult rolling_zscore(std::span<const double> values, int window) {
    if (window <= 1) throw std::invalid_argument("window must be > 1");
    if (values.empty()) return {};

    size_t n = values.size();
    ZScoreResult result;
    result.zscore.resize(n, 0.0);
    result.rolling_mean.resize(n, 0.0);
    result.rolling_std.resize(n, 0.0);

    // Online computation using Welford's algorithm for numerical stability
    for (size_t i = 0; i < n; ++i) {
        size_t start = (i >= static_cast<size_t>(window)) ? i - window + 1 : 0;
        size_t count = i - start + 1;

        // Compute mean and variance over the window
        double sum = 0.0;
        for (size_t j = start; j <= i; ++j) {
            sum += values[j];
        }
        double mean = sum / count;

        double var_sum = 0.0;
        for (size_t j = start; j <= i; ++j) {
            double diff = values[j] - mean;
            var_sum += diff * diff;
        }
        double std_dev = (count > 1) ? std::sqrt(var_sum / (count - 1)) : 0.0;

        result.rolling_mean[i] = mean;
        result.rolling_std[i] = std_dev;
        result.zscore[i] = (std_dev > 1e-12)
            ? (values[i] - mean) / std_dev
            : 0.0;
    }

    return result;
}

double zscore_single(double value, double rolling_mean, double rolling_std) {
    if (rolling_std < 1e-12) return 0.0;
    return (value - rolling_mean) / rolling_std;
}

} // namespace grove::signals
