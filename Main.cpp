#include <iostream>
#include <cmath>

using namespace std;

// Black-Scholes formula for European call option price
double call_option_price(double S, double K, double r, double T, double sigma) {
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return S * exp(-r * T) * norm_cdf(d1) - K * exp(-r * T) * norm_cdf(d2);
}

// Normal cumulative distribution function (CDF)
double norm_cdf(double x) {
    return 0.5 * erfc(-x / sqrt(2));
}

// Function to calculate the difference between market price and theoretical price
double call_option_price_diff(double S, double K, double r, double T, double sigma, double market_price) {
    return call_option_price(S, K, r, T, sigma) - market_price;
}

// Function to find implied volatility using Secant Method
double implied_volatility(double S, double K, double r, double T, double market_price, double initial_guess) {
    const double tolerance = 1.0e-6;
    const int max_iterations = 1000;

    double x0 = initial_guess;
    double x1 = x0 * 1.1; // Initial second guess

    for (int i = 0; i < max_iterations; ++i) {
        double y0 = call_option_price_diff(S, K, r, T, x0, market_price);
        double y1 = call_option_price_diff(S, K, r, T, x1, market_price);

        double x_next = x1 - y1 * (x1 - x0) / (y1 - y0);

        if (abs(x_next - x1) < tolerance)
            return x_next;

        x0 = x1;
        x1 = x_next;
    }

    cerr << "Failed to converge to the required tolerance within max iterations." << endl;
    return NAN;
}

int main() {
    // Example usage
    double S = 100.0;       // Underlying price
    double K = 100.0;       // Strike price
    double r = 0.05;        // Risk-free rate
    double T = 1.0;         // Time to maturity
    double market_price = 5.0; // Market price of the option
    double initial_guess = 0.2; // Initial guess for volatility

    double implied_vol = implied_volatility(S, K, r, T, market_price, initial_guess);

    if (!isnan(implied_vol)) {
        cout << "Implied volatility: " << implied_vol * 100 << "%" << endl;
    }

    return 0;
}
