#include "grove/signals/moving_average.h"

#include <stdexcept>

namespace grove::signals {

std::vector<double> ema(std::span<const double> prices, int period) {
    if (period <= 0) throw std::invalid_argument("period must be positive");
    if (prices.empty()) return {};

    size_t n = prices.size();
    std::vector<double> result(n);
    double alpha = 2.0 / (period + 1);

    // Seed with first value
    result[0] = prices[0];
    for (size_t i = 1; i < n; ++i) {
        result[i] = alpha * prices[i] + (1.0 - alpha) * result[i - 1];
    }

    return result;
}

std::vector<double> sma(std::span<const double> prices, int period) {
    if (period <= 0) throw std::invalid_argument("period must be positive");
    if (prices.empty()) return {};

    size_t n = prices.size();
    std::vector<double> result(n, 0.0);

    double running_sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        running_sum += prices[i];
        if (i >= static_cast<size_t>(period)) {
            running_sum -= prices[i - period];
        }
        size_t count = std::min(i + 1, static_cast<size_t>(period));
        result[i] = running_sum / count;
    }

    return result;
}

std::vector<double> dema(std::span<const double> prices, int period) {
    auto ema1 = ema(prices, period);
    auto ema2 = ema(std::span<const double>(ema1), period);

    std::vector<double> result(prices.size());
    for (size_t i = 0; i < prices.size(); ++i) {
        result[i] = 2.0 * ema1[i] - ema2[i];
    }
    return result;
}

std::vector<int> ema_crossover(std::span<const double> prices,
                                int fast_period, int slow_period) {
    auto fast = ema(prices, fast_period);
    auto slow = ema(prices, slow_period);

    std::vector<int> signal(prices.size(), 0);
    for (size_t i = 1; i < prices.size(); ++i) {
        if (fast[i] > slow[i] && fast[i - 1] <= slow[i - 1]) {
            signal[i] = 1;   // bullish crossover
        } else if (fast[i] < slow[i] && fast[i - 1] >= slow[i - 1]) {
            signal[i] = -1;  // bearish crossover
        }
    }
    return signal;
}

} // namespace grove::signals
