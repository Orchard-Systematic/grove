#include "grove/curves/seasonal_decomposition.h"

#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace grove::curves {

SeasonalDecomposition decompose_additive(const std::vector<double>& values,
                                          int period) {
    if (period <= 0) throw std::invalid_argument("period must be positive");
    if (values.size() < static_cast<size_t>(2 * period)) {
        throw std::invalid_argument("need at least 2 full periods of data");
    }

    size_t n = values.size();
    SeasonalDecomposition result;
    result.period = period;
    result.trend.resize(n, 0.0);
    result.seasonal.resize(n, 0.0);
    result.residual.resize(n, 0.0);

    // Step 1: Compute centred moving average for trend
    int half = period / 2;
    for (size_t i = static_cast<size_t>(half); i + half < n; ++i) {
        double sum = 0.0;
        for (int j = -half; j <= half; ++j) {
            double weight = (std::abs(j) == half && period % 2 == 0) ? 0.5 : 1.0;
            sum += weight * values[i + j];
        }
        double divisor = (period % 2 == 0) ? period : period;
        result.trend[i] = sum / divisor;
    }

    // Extend trend at boundaries with flat extrapolation
    for (size_t i = 0; i < static_cast<size_t>(half); ++i) {
        result.trend[i] = result.trend[half];
    }
    for (size_t i = n - half; i < n; ++i) {
        result.trend[i] = result.trend[n - half - 1];
    }

    // Step 2: Detrend and compute seasonal indices
    std::vector<double> detrended(n);
    for (size_t i = 0; i < n; ++i) {
        detrended[i] = values[i] - result.trend[i];
    }

    auto indices = seasonal_indices(detrended, period);

    // Step 3: Apply seasonal pattern
    for (size_t i = 0; i < n; ++i) {
        result.seasonal[i] = indices[i % period];
        result.residual[i] = values[i] - result.trend[i] - result.seasonal[i];
    }

    return result;
}

std::vector<double> seasonal_indices(const std::vector<double>& values,
                                     int period) {
    if (period <= 0) throw std::invalid_argument("period must be positive");

    std::vector<double> indices(period, 0.0);
    std::vector<int> counts(period, 0);

    for (size_t i = 0; i < values.size(); ++i) {
        int idx = static_cast<int>(i) % period;
        indices[idx] += values[i];
        counts[idx]++;
    }

    for (int i = 0; i < period; ++i) {
        if (counts[i] > 0) {
            indices[i] /= counts[i];
        }
    }

    // Centre the indices (sum to zero for additive model)
    double mean = std::accumulate(indices.begin(), indices.end(), 0.0) / period;
    for (auto& v : indices) {
        v -= mean;
    }

    return indices;
}

std::vector<double> deseasonalise(const std::vector<double>& values,
                                   const std::vector<double>& indices) {
    std::vector<double> result(values.size());
    int period = static_cast<int>(indices.size());
    for (size_t i = 0; i < values.size(); ++i) {
        result[i] = values[i] - indices[i % period];
    }
    return result;
}

} // namespace grove::curves
