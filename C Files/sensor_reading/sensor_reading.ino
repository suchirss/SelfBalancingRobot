#include "Arduino_BMI270_BMM150.h"

void setup() {
  IMU.begin();
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
}
}

void loop() {
  // put your main code here, to run repeatedly:
  float xa, ya, za;
  float xg, yg, zg;
  unsigned long currentTime = millis();
  IMU.readAcceleration(xa,ya,za);
  IMU.readGyroscope(xg,yg,zg);
    
  Serial.print(xa);
  Serial.print('\t');
  Serial.print(ya);
  Serial.print('\t');
  Serial.print(za);
  Serial.print('\t');
  Serial.print(xg);
  Serial.print('\t');
  Serial.print(yg);
  Serial.print('\t');
  Serial.print(zg);
  Serial.print('\t');
  Serial.println(currentTime);
} 
