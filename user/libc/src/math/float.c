#include <math.h>
#include <stdint.h>

#define LN2_F 0.6931471805599453f
#define INV_LN2_F 1.4426950408889634f
#define HALF_LN2_F 0.34657359027997264f

static float cf_logf_approx(float x) {
    union {
        float f;
        uint32_t u;
    } bits;
    float y;
    float y2;
    int exp;

    if (x <= 0.0f) {
        return -1.0e30f;
    }

    bits.f = x;
    exp = (int)((bits.u >> 23) & 0xffU) - 127;
    bits.u = (bits.u & 0x007fffffU) | 0x3f800000U;

    y = (bits.f - 1.0f) / (bits.f + 1.0f);
    y2 = y * y;

    return ((float)exp * LN2_F) +
           (2.0f * (y +
                    (y * y2) / 3.0f +
                    (y * y2 * y2) / 5.0f +
                    (y * y2 * y2 * y2) / 7.0f +
                    (y * y2 * y2 * y2 * y2) / 9.0f));
}

static float cf_pow2f(int n) {
    union {
        uint32_t u;
        float f;
    } bits;

    if (n < -126) {
        return 0.0f;
    }
    if (n > 127) {
        return 3.402823466e38f;
    }

    bits.u = (uint32_t)(n + 127) << 23;
    return bits.f;
}

float sinf(float x) {
    return (float)sin((double)x);
}

float cosf(float x) {
    return (float)cos((double)x);
}

float sqrtf(float x) {
    return (float)sqrt((double)x);
}

float fabsf(float x) {
    return x < 0.0f ? -x : x;
}

float expf(float x) {
    float r;
    float poly;
    int n;
    float scaled;
    float r2;

    if (x <= -103.0f) {
        return 0.0f;
    }
    if (x >= 88.0f) {
        return 3.402823466e38f;
    }

    scaled = x * INV_LN2_F;
    n = (int)(scaled + (scaled >= 0.0f ? 0.5f : -0.5f));
    r = x - ((float)n * LN2_F);
    if (r > HALF_LN2_F) {
        n += 1;
        r -= LN2_F;
    } else if (r < -HALF_LN2_F) {
        n -= 1;
        r += LN2_F;
    }

    r2 = r * r;
    poly = 1.0f + r +
           r2 * (0.5f +
           r * (1.0f / 6.0f +
           r * (1.0f / 24.0f +
           r * (1.0f / 120.0f +
           r * (1.0f / 720.0f +
           r * (1.0f / 5040.0f))))));

    return poly * cf_pow2f(n);
}

float tanhf(float x) {
    float ax;
    float e;

    if (x > 8.0f) {
        return 1.0f;
    }
    if (x < -8.0f) {
        return -1.0f;
    }

    ax = fabsf(x);
    e = expf(-2.0f * ax);
    return x >= 0.0f ? (1.0f - e) / (1.0f + e) : (e - 1.0f) / (1.0f + e);
}

float powf(float x, float y) {
    if (x == 0.0f) {
        return y > 0.0f ? 0.0f : 1.0f;
    }
    if (x < 0.0f) {
        int yi = (int)y;
        float result = 1.0f;
        float base = x;
        int exp_int;

        if ((float)yi != y) {
            return 0.0f;
        }

        exp_int = yi < 0 ? -yi : yi;
        while (exp_int > 0) {
            if ((exp_int & 1) != 0) {
                result *= base;
            }
            base *= base;
            exp_int >>= 1;
        }

        return yi < 0 ? 1.0f / result : result;
    }

    return expf(y * cf_logf_approx(x));
}

float roundf(float x) {
    return x >= 0.0f ? (float)((int)(x + 0.5f)) : (float)((int)(x - 0.5f));
}

double exp(double x) {
    return (double)expf((float)x);
}

double tanh(double x) {
    return (double)tanhf((float)x);
}

double pow(double x, double y) {
    return (double)powf((float)x, (float)y);
}
