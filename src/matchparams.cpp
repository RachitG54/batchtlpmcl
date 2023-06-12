#include <iostream>
#include <cmath>

using namespace std;

double logBinomialCoefficient(int n, int k) {
    double result = 0.0;

    // Since C(n, k) = C(n, n-k)
    if (k > n - k)
        k = n - k;

    // Calculate logarithmic value of [n*(n-1)*...*(n-k+1)] / [k*(k-1)*...*1]
    for (int i = 0; i < k; ++i) {
        result += log((n - i) * 1.0 / (i + 1));
    }

    return result;
}

double evaluateExpression(int n, int q, int m, int d) {
    double result = 0.0;

    for (int i = d; i <= n; ++i) {
        double term = logBinomialCoefficient(q, i) +
                      logBinomialCoefficient(m, i - 1) +
                      i * d * log((i - 1.0) / m);

        result += term;
    }

    return result;
}

int main() {
    int n, q, m, d;
    cout << "Enter the values of n, q, m, and d: ";
    cin >> n >> q >> m >> d;

    double logResult = evaluateExpression(n, q, m, d);
    cout << "log x = " << logResult << endl;

    return 0;
}