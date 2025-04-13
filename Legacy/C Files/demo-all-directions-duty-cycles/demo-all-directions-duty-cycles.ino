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

  Serial.begin(9600);
}

// 100% (Forward, Reverse)
// 75% (Forward, Reverse, Alternate1, Alternate2)
// 50% (Forward, Reverse)
// 25% (Forward, Reverse, Alternate1, Alternate2)

void loop() {
    int dutyCycle = 55; // results in max RPM of 420 RPM
    int pwmValue = dutyToPWM(dutyCycle);
    
    // Print speed value for debugging
//    Serial.print("PWM Value: ");
//    Serial.println(pwmValue);

    moveWheelsForward(pwmValue);
    Serial.println("FORWARD MAX RPM");
    delay(7000);  // Small delay for stability

    moveWheelsBackward(pwmValue);
    Serial.println("BACKWARD MAX RPM");
    delay(7000);

    stopWheels();
    delay(5000);

// ------------------------------------------------------------------------------------------------------------

    dutyCycle = 31;
    pwmValue = dutyToPWM(dutyCycle);
    
    // Print speed value for debugging
//    Serial.print("PWM Value: ");
//    Serial.println(pwmValue);

    moveWheelsForward(pwmValue);
    Serial.println("FORWARD 75% RPM");
    delay(7000);  // Small delay for stability

    moveWheelsBackward(pwmValue);
    Serial.println("BACKWARD 75% RPM");
    delay(7000);

    moveWheelsAlternate1(pwmValue);
    Serial.println("ALTERNATE 1 - 75% RPM");
    delay(7000);

    moveWheelsAlternate2(pwmValue);
    Serial.println("ALTERNATE 2 - 75% RPM");
    delay(7000);

    stopWheels();
    delay(5000);

 // ------------------------------------------------------------------------------------------------------------

    dutyCycle = 21;
    pwmValue = dutyToPWM(dutyCycle);
    
    // Print speed value for debugging
//    Serial.print("PWM Value: ");
//    Serial.println(pwmValue);

//    moveWheelsForward(pwmValue);
//    delay(7000);  // Small delay for stability
//
//    moveWheelsBackward(pwmValue);
//    delay(7000);

    moveWheelsBackwardFastDecay(pwmValue);
    Serial.println("BACKWARD 50% RPM");
    delay(7000);

    moveWheelsForwardFastDecay(pwmValue);
    Serial.println("FORWARD 50% RPM");
    delay(7000);  // Small delay for stability

    
//    commenting out the stop wheels function to avoid motor from stalling when going from 50% to 25% of max RPM
//    stopWheels();
//    delay(5000);

// ------------------------------------------------------------------------------------------------------------

    dutyCycle = 18;
    pwmValue = dutyToPWM(dutyCycle);
    
    // Print speed value for debugging
//    Serial.print("PWM Value: ");
//    Serial.println(pwmValue);

//    moveWheelsForward(pwmValue);
//    delay(7000);  // Small delay for stability
//
//    moveWheelsBackward(pwmValue);
//    delay(7000);
   
    moveWheelsForwardFastDecay(pwmValue);
    Serial.println("FORWARD 25% RPM");
    delay(7000);  // Small delay for stability

    moveWheelsBackwardFastDecay(pwmValue);
    Serial.println("BACKWARD 25% RPM");
    delay(7000);

    moveWheelsAlternate1(pwmValue);
    Serial.println("ALTERNATE 1 - 25% RPM");
    delay(7000);

    moveWheelsAlternate2(pwmValue);
    Serial.println("ALTERNATE 2 - 25% RPM");
    delay(7000);

    stopWheels();
    delay(5000);

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
