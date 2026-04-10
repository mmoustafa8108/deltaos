#include <math.h>

double mod(double a, double b) {
    if (b == 0.0) {
        return 0.0;
    }
    return a - (floor(a / b) * b);
}
