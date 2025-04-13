#include "Arduino_BMI270_BMM150.h"
#include <math.h>  // Required for atan2()
#include <stdio.h>

// Define motor control pins
  const int pwmA1 = 9;   // PWM output for Motor A
  const int pwmA2 = 10;  // PWM output for Motor A
  const int pwmB1 = 5;   // PWM output for Motor B
  const int pwmB2 = 6;   // PWM output for Motor B

void setup() {  
  
  pinMode(pwmA1, OUTPUT);
  pinMode(pwmA2, OUTPUT);
  pinMode(pwmB1, OUTPUT); 
  pinMode(pwmB2, OUTPUT);
  
    Serial.begin(57600);  // Ensure baud rate matches Python
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
}

void loop() {
    float xa, ya, za;
    float xg, yg, zg;
    unsigned long currentTime = millis();

    // **Read sensor values**
    IMU.readAcceleration(xa, ya, za);
    IMU.readGyroscope(xg, yg, zg);

    // **Compute tilt angle using accelerometer data**
    float tilt_angle = atan2(ya, za) * 180.0 / M_PI;  // Convert to degrees

        // **Print tilt angle separately with separator**
    Serial.println("---------------");
    Serial.print("Tilt Angle (Degrees): ");
    Serial.println(tilt_angle);
    Serial.println("---------------");

    // **Send sensor data + calculated tilt angle to Python**
    Serial.print(xa); Serial.print('\t');
    Serial.print(ya); Serial.print('\t');
    Serial.print(za); Serial.print('\t');
    Serial.print(xg); Serial.print('\t');
    Serial.print(yg); Serial.print('\t');
    Serial.print(zg); Serial.print('\t');
    Serial.print(tilt_angle); Serial.print('\t');  // Send tilt angle
    Serial.println(currentTime);
    
    delay(10);  // Small delay to avoid spamming

    moveWheelsBasedOnTiltAngle(tilt_angle);
}

int dutyToPWM(int dutyCycle) {
  return map(dutyCycle, 0, 100, 0, 255);
}

void moveWheelsForward(int pwmInput) {
  // Apply PWM signal to motor
  // PWM is slow decay
    analogWrite(pwmA1, 1);
    analogWrite(pwmA2, pwmInput);
    analogWrite(pwmB1, 1);
    analogWrite(pwmB2, pwmInput);
}

void moveWheelsForwardFastDecay(int pwmInput) {
  // Apply PWM signal to motor
  // PWM is slow decay
    analogWrite(pwmA1, pwmInput);
    analogWrite(pwmA2, 0);
    analogWrite(pwmB1, pwmInput);
    analogWrite(pwmB2, 0);
}

void moveWheelsBackward(int pwmInput) {
  // Apply PWM signal to motor
  // PWM is slow decay
    analogWrite(pwmA1, pwmInput);
    analogWrite(pwmA2, 1);
    analogWrite(pwmB1, pwmInput);
    analogWrite(pwmB2, 1);
}

void moveWheelsBackwardFastDecay(int pwmInput) {
  // Apply PWM signal to motor
  // PWM is slow decay
    analogWrite(pwmA1, 0);
    analogWrite(pwmA2, pwmInput);
    analogWrite(pwmB1, 0);
    analogWrite(pwmB2, pwmInput);
}

void moveWheelsAlternate1(int pwmInput) {
  // Apply PWM signal to motor
  // PWM is slow decay
    analogWrite(pwmA1, 1);
    analogWrite(pwmA2, pwmInput);
    analogWrite(pwmB1, pwmInput);
    analogWrite(pwmB2, 1);
}

void moveWheelsAlternate2(int pwmInput) {
  // Apply PWM signal to motor
  // PWM is slow decay
    analogWrite(pwmA1, pwmInput);
    analogWrite(pwmA2, 1);
    analogWrite(pwmB1, 1);
    analogWrite(pwmB2, pwmInput);
}

void stopWheels() {
  // Apply PWM signal to motor
  // PWM is slow decay
    analogWrite(pwmA1, 0);
    analogWrite(pwmA2, 0);
    analogWrite(pwmB1, 0);
    analogWrite(pwmB2, 0);
}

// --------------------------------------------------------------------------------

int solvePWM(double a, double b, double c) {
    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        double root1 = (-b + sqrt(discriminant)) / (2 * a);
        double root2 = (-b - sqrt(discriminant)) / (2 * a);
        return min(root1, root2);
    } else if (discriminant == 0) {
        double root = -b / (2 * a);
        return root;
    }
    else {
      return 0;
    }
}


void moveWheelsBasedOnTiltAngle(float tiltAngle) {
    int newTiltAngle = abs(tiltAngle);
//    float difference = abs(fmod(tiltAngle,90.0));

    int rpm = map(newTiltAngle, 2, 90, 200, 350);
    int dutyCycle = solvePWM(-0.1832, 19.797, -120.17 - rpm); // you want this in the form of ax^2 + bx + c = 0. since we have ax^2 + bx + c = RPM, we subtract RPM on both sides.
    int pwm = map(dutyCycle, 20, 55, 51, 190);

    if (tiltAngle < 90 && tiltAngle > 2) {
      Serial.print("tilt angle > 90 and > 2\n");
      Serial.print(pwm);
      Serial.print("\n");
      Serial.print(rpm);
        // move the wheel forward
        moveWheelsForwardFastDecay(pwm);
    }
    else if (tiltAngle > -90 && tiltAngle < -2){
        // move wheels backward
        Serial.print("tilt angle > -90 and > -2\n");
        Serial.print(pwm);
        Serial.print("\n");
        Serial.print(rpm);
        moveWheelsBackwardFastDecay(pwm);
    }
    else {
       Serial.print("tilt angle > 90 and > 2\n");

        stopWheels();
    }

}
