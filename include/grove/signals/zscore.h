#pragma once

#include <span>
#include <vector>

namespace grove::signals {

/// Rolling z-score for mean-reversion signals.

struct ZScoreResult {
    std::vector<double> zscore;
    std::vector<double> rolling_mean;
    std::vector<double> rolling_std;
};

/// Compute rolling z-score over a lookback window.
/// z = (x - mean) / std
/// @param values   Input series
/// @param window   Lookback window size
/// @return         Z-score result with mean and std components
ZScoreResult rolling_zscore(std::span<const double> values, int window);

/// Compute z-score of the current value relative to a rolling window.
/// Useful for single-tick updates.
double zscore_single(double value, double rolling_mean, double rolling_std);

} // namespace grove::signals
