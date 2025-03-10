#include "calculate_tilt_angle_functions.h"
#include <math.h>

#define PI 3.14159265358979323846

float radians_to_degrees(int radians) {
    return radians * 180/PI;
}

float calc_acc_tilt_angle(int ya, int za) {
    float acc_tilt_angle;

    if(za != 0) {
        float yzratio = float(ya/za);
        if ((fabs(za) <=0.04) && (fabs(ya) <=0.04)) {
            acc_tilt_angle = radians_to_degrees(yzratio);
        }
        else {
            acc_tilt_angle = radians_to_degrees((yzratio));
        }
    }

    return acc_tilt_angle;
}