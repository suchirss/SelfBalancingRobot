#ifndef CALCULATE_TILT_ANGLE_FUNCTIONS_H
#define CALCULATE_TILT_ANGLE_FUNCTIONS_H

#include "Arduino_BMI270_BMM150.h"

float calc_acc_tilt_angle(float ay, float az);
float calc_gyro_change_angle(float last_timestamp, float current_timestamp, float xg);
float calc_comp_angle(float k, float tilt_angle_gyro, float tilt_angle_acc, float prevComp);

#endif
