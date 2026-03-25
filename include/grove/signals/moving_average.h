#pragma once

#include <cstddef>
#include <span>
#include <vector>

namespace grove::signals {

/// Fast exponential moving average (EMA).
/// Processes an entire series in a single pass — designed for nanobind
/// zero-copy exposure to NumPy arrays.
/// @param prices  Input price series
/// @param period  EMA lookback period
/// @return        EMA values (same length as input, first `period-1` are warm-up)
std::vector<double> ema(std::span<const double> prices, int period);

/// Simple moving average (SMA).
std::vector<double> sma(std::span<const double> prices, int period);

/// Double EMA (DEMA) — reduced lag.
std::vector<double> dema(std::span<const double> prices, int period);

/// EMA crossover signal: +1 when fast > slow, -1 when fast < slow, 0 otherwise.
std::vector<int> ema_crossover(std::span<const double> prices,
                                int fast_period, int slow_period);

} // namespace grove::signals
