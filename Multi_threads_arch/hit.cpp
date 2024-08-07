#include "hit.h"

bool hit_test(float x, float y, float z) {
    float a = 2.0;
    return a*a*(y*y + z*z) - a*x*x*x + x*x*x*x <= 0;
}

const float* get_axis_range() {
    float a = 2.0;
    float* range = new float[6];
    range[0] = -2*a;
    range[2] = -2*a;
    range[4] = -2*a;
    range[1] = 2*a;
    range[3] = 2*a;
    range[5] = 2*a;
    return range;
}
