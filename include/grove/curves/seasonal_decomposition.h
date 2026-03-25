#pragma once

#include <vector>

namespace grove::curves {

/// Seasonal decomposition for commodity forward curves.
/// Decomposes a price series into trend + seasonal + residual components.
/// Uses classical additive decomposition with a moving average for trend.

struct SeasonalDecomposition {
    std::vector<double> trend;
    std::vector<double> seasonal;
    std::vector<double> residual;
    int period;
};

/// Perform additive seasonal decomposition.
/// @param values      Input time series
/// @param period      Seasonal period (e.g. 12 for monthly data, 252 for daily)
/// @return            Decomposed components
SeasonalDecomposition decompose_additive(const std::vector<double>& values,
                                          int period);

/// Compute seasonal indices from historical price data.
/// @param values      Input time series (must span multiple full periods)
/// @param period      Seasonal period
/// @return            Vector of length `period` with average seasonal factors
std::vector<double> seasonal_indices(const std::vector<double>& values,
                                     int period);

/// Deseasonalise a price series given known seasonal indices.
std::vector<double> deseasonalise(const std::vector<double>& values,
                                   const std::vector<double>& indices);

} // namespace grove::curves
