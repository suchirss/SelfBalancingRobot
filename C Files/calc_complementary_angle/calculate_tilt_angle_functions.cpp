#include "calculate_tilt_angle_functions.h"
#include <math.h>

#define PI 3.14159265358979323846

float radians_to_degrees(int radians) {
    return radians * 180/PI;
}

float calc_acc_tilt_angle(int ay, int az) {
    float acc_tilt_angle;

    if(az != 0) {
        float yzratio = float(ay/az);
        if ((fabs(ay) <=0.04) && (fabs(az) <=0.04)) {
            acc_tilt_angle = radians_to_degrees(yzratio);
        }
        else {
            acc_tilt_angle = radians_to_degrees((yzratio));
        }
    }

    return acc_tilt_angle;
}

float calc_gyro_tilt_angle(float last_gyro_angle, float last_timestamp, float current_timestamp, float gz) {
        float gyro_tilt_angle = last_gyro_angle + ( (current_timestamp - last_timestamp) * gz );

    return gyro_tilt_angle;
}
