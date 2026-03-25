#include "grove/pricing/black76.h"

#include <cmath>
#include <algorithm>

namespace grove::pricing {

namespace {

// Standard normal CDF (Abramowitz and Stegun approximation).
double norm_cdf(double x) {
    return 0.5 * std::erfc(-x * M_SQRT1_2);
}

// Standard normal PDF.
double norm_pdf(double x) {
    static constexpr double INV_SQRT_2PI = 0.3989422804014327;
    return INV_SQRT_2PI * std::exp(-0.5 * x * x);
}

struct D1D2 {
    double d1, d2;
};

D1D2 compute_d1d2(double F, double K, double T, double sigma) {
    double sqrt_T = std::sqrt(T);
    double d1 = (std::log(F / K) + 0.5 * sigma * sigma * T) / (sigma * sqrt_T);
    double d2 = d1 - sigma * sqrt_T;
    return {d1, d2};
}

} // anonymous namespace

Black76Result black76_call(double F, double K, double r, double T, double sigma) {
    if (T <= 0.0 || sigma <= 0.0) {
        double intrinsic = std::max(F - K, 0.0);
        return {intrinsic, (F > K) ? 1.0 : 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    auto [d1, d2] = compute_d1d2(F, K, T, sigma);
    double df = std::exp(-r * T);
    double sqrt_T = std::sqrt(T);

    double price = df * (F * norm_cdf(d1) - K * norm_cdf(d2));
    double delta = df * norm_cdf(d1);
    double gamma = df * norm_pdf(d1) / (F * sigma * sqrt_T);
    double vega = df * F * norm_pdf(d1) * sqrt_T * 0.01;  // per 1% vol move
    double theta = (-df * F * norm_pdf(d1) * sigma / (2.0 * sqrt_T)
                    - r * price) / 365.0;  // per calendar day
    double rho = -T * price * 0.01;  // per 1% rate move

    return {price, delta, gamma, vega, theta, rho};
}

Black76Result black76_put(double F, double K, double r, double T, double sigma) {
    if (T <= 0.0 || sigma <= 0.0) {
        double intrinsic = std::max(K - F, 0.0);
        return {intrinsic, (K > F) ? -1.0 : 0.0, 0.0, 0.0, 0.0, 0.0};
    }

    auto [d1, d2] = compute_d1d2(F, K, T, sigma);
    double df = std::exp(-r * T);
    double sqrt_T = std::sqrt(T);

    double price = df * (K * norm_cdf(-d2) - F * norm_cdf(-d1));
    double delta = -df * norm_cdf(-d1);
    double gamma = df * norm_pdf(d1) / (F * sigma * sqrt_T);
    double vega = df * F * norm_pdf(d1) * sqrt_T * 0.01;
    double theta = (-df * F * norm_pdf(d1) * sigma / (2.0 * sqrt_T)
                    - r * price) / 365.0;
    double rho = -T * price * 0.01;

    return {price, delta, gamma, vega, theta, rho};
}

double black76_implied_vol(double market_price, double F, double K,
                           double r, double T, bool is_call,
                           double tol, int max_iter) {
    if (T <= 0.0 || market_price <= 0.0) return std::nan("");

    // Newton-Raphson starting from 0.3 (30% vol)
    double sigma = 0.3;
    double df = std::exp(-r * T);
    double sqrt_T = std::sqrt(T);

    for (int i = 0; i < max_iter; ++i) {
        auto [d1, d2] = compute_d1d2(F, K, T, sigma);

        double model_price;
        if (is_call) {
            model_price = df * (F * norm_cdf(d1) - K * norm_cdf(d2));
        } else {
            model_price = df * (K * norm_cdf(-d2) - F * norm_cdf(-d1));
        }

        double vega_raw = df * F * norm_pdf(d1) * sqrt_T;
        if (vega_raw < 1e-12) break;

        double diff = model_price - market_price;
        if (std::abs(diff) < tol) return sigma;

        sigma -= diff / vega_raw;
        sigma = std::max(sigma, 1e-6);  // keep vol positive
    }

    return std::nan("");  // did not converge
}

} // namespace grove::pricing
