#ifndef CALCULATE_TILT_ANGLE_FUNCTIONS_H
#define CALCULATE_TILT_ANGLE_FUNCTIONS_H

float calc_acc_tilt_angle(int ya, int yz);
float calc_gyro_tilt_angle(float last_gyro_angle, float last_timestamp, float current_timestamp, float zg);

#endif
