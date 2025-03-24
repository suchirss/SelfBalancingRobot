#include "calculate_tilt_angle_functions.h" 
#include <math.h>

#define PI 3.14159265358979323846

float radians_to_degrees(float radians) {
    return radians * 180/PI;
}

float calc_acc_tilt_angle(float ay, float az) {
    float acc_tilt_angle;

    if(az != 0) {
        float yzratio = float(ay/az);
        if ((fabs(ay) <=0.04) && (fabs(az) <=0.04)) {
            acc_tilt_angle = radians_to_degrees(atan(yzratio));
        }
        else {
            acc_tilt_angle = radians_to_degrees((atan(yzratio)));
        }
    }

    return acc_tilt_angle;
}

float calc_gyro_tilt_angle(float last_gyro_angle, float last_timestamp, float current_timestamp, float xg) {
        float gyro_tilt_angle = last_gyro_angle - ( (current_timestamp/1000.0 - last_timestamp/1000.0) * xg );

    return gyro_tilt_angle;
}

float calc_gyro_change_angle(float last_timestamp, float current_timestamp, float xg) {
  float gyro_change_angle = (current_timestamp/1000.0 - last_timestamp/1000.0) * xg;

  return gyro_change_angle;
}

float calc_comp_angle(float k, float gyro_change_angle, float tilt_angle_acc, float prevComp) {
  float newComplementaryAngle = (k * (prevComp + gyro_change_angle)) + ((1 - k) * tilt_angle_acc);
  return newComplementaryAngle;
}
