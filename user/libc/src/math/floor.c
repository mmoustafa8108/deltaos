double floor(double x) {
    long long i = (long long)x;
    if ((double)i > x) {
        i--;
    }
    return (double)i;
}
