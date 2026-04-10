#include <math.h>

double sqrt(double x) {
    double guess;

    if (x <= 0.0) {
        return 0.0;
    }

    guess = x > 1.0 ? x : 1.0;
    for (int i = 0; i < 16; ++i) {
        guess = 0.5 * (guess + (x / guess));
    }
    return guess;
}
