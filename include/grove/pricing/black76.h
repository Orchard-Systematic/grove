#pragma once

#include <cmath>

namespace grove::pricing {

/// Black-76 model for pricing European options on futures.
/// Standard model for commodity futures options.
struct Black76Result {
    double price;
    double delta;
    double gamma;
    double vega;
    double theta;
    double rho;
};

/// Calculate call option price under Black-76.
/// @param F     Forward/futures price
/// @param K     Strike price
/// @param r     Risk-free rate (annualised)
/// @param T     Time to expiry (years)
/// @param sigma Implied volatility (annualised)
/// @return      Full Greeks result
Black76Result black76_call(double F, double K, double r, double T, double sigma);

/// Calculate put option price under Black-76.
Black76Result black76_put(double F, double K, double r, double T, double sigma);

/// Implied volatility from market price via Newton-Raphson.
/// @param market_price Observed option price
/// @param F            Forward/futures price
/// @param K            Strike price
/// @param r            Risk-free rate
/// @param T            Time to expiry
/// @param is_call      true for call, false for put
/// @param tol          Convergence tolerance
/// @param max_iter     Maximum iterations
/// @return             Implied vol, or NaN if not converged
double black76_implied_vol(double market_price, double F, double K,
                           double r, double T, bool is_call,
                           double tol = 1e-8, int max_iter = 100);

} // namespace grove::pricing
