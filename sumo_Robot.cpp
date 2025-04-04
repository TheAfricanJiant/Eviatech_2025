/******************************************************************
 * Sumo Robot Control Software
 * -----
 * Based on ElviaTech National Robotics Competition Code
 * -----
 * Goal: Fast and effective sumo robot implementation
 * Updated: Using only front and back IR sensors
 * Author : Tambu Precious, 2025
 *******************************************************************/

 #include "config.h"

 // State machine states
 enum State {
   SEARCH,
   TRACK,
   ATTACK,
   EVADE
 };
 
 // Current robot state
 State currentState = SEARCH;
 
 // Thresholds and constants
 const int ATTACK_THRESHOLD = 30;    // Distance in cm to trigger attack mode
 const int TRACK_THRESHOLD = 60;     // Distance in cm to trigger tracking
 const int BOUNDARY_THRESHOLD = 500; // IR threshold for ring boundary (adjust based on your sensors)
 const int EVADE_TIME = 500;         // Time in ms for evade maneuver
 const int SEARCH_TURN_TIME = 800;   // Time for search turning
 
 // Using only two IR sensors - front and back
 const uint8_t IR_FRONT = IR_2;      // Front IR sensor (using IR_2 from config)
 const uint8_t IR_BACK = IR_4;       // Back IR sensor (using IR_4 from config)
 
 // Timing variables
 unsigned long lastStateChangeTime = 0;
 unsigned long searchStartTime = 0;
 unsigned long attackStartTime = 0;
 unsigned long evadeStartTime = 0;

 //const uint8_t IR_2 = A6;
//const uint8_t IR_3 = A3;
//const uint8_t IR_4 = A7; // right IR sensor

 
 // Search pattern variables
 bool searchDirection = true; // true = right, false = left
 int searchPhase = 0;
 unsigned long lastSearchPatternChange = 0;
 
 // Function prototypes
 void setMotorSpeeds(int leftSpeed, int rightSpeed);
 void measureDistances();
 bool checkBoundary(uint8_t &detectedSensor);
 void executeSearch();
 void executeTrack();
 void executeAttack();
 void executeEvade(uint8_t detectedSensor);
 void updateState();
 
 void setup() {
   // Initialize Serial for debugging
   Serial.begin(115200);
   
   // Initialize motor pins
   pinMode(rightMotorA, OUTPUT);
   pinMode(rightMotorB, OUTPUT);
   pinMode(rightMotorPWM, OUTPUT);
   pinMode(leftMotorA, OUTPUT);
   pinMode(leftMotorB, OUTPUT);
   pinMode(leftMotorPWM, OUTPUT);
   
   // Initialize IR sensors as inputs (only using front and back)
   pinMode(IR_FRONT, INPUT);
   pinMode(IR_BACK, INPUT);
   
   // Initialize ultrasonic sensor pins
   pinMode(R_trig, OUTPUT);
   pinMode(R_echo, INPUT);
   pinMode(F_trig, OUTPUT);
   pinMode(F_echo, INPUT);
   pinMode(L_trig, OUTPUT);
   pinMode(L_echo, INPUT);
   
   // Set up interrupts for ultrasonic sensors
   attachInterrupt(digitalPinToInterrupt(R_echo), R_echoISR, CHANGE);
   attachInterrupt(digitalPinToInterrupt(F_echo), F_echoISR, CHANGE);
   attachInterrupt(digitalPinToInterrupt(L_echo), L_echoISR, CHANGE);
   
   // Initialize control button pins
   pinMode(button1, INPUT_PULLUP);
   pinMode(button2, INPUT_PULLUP);
   
   // Initialize on-board LED
   pinMode(onboardLED, OUTPUT);
   
   // Wait for button press to start
   digitalWrite(onboardLED, HIGH);
   while (digitalRead(button1) == HIGH) {
     delay(10);
   }
   
   // Button pressed, start the robot
   digitalWrite(onboardLED, LOW);
   delay(5000);  // 5-second delay to place robot
   digitalWrite(onboardLED, HIGH);
 }
 
 void loop() {
   // Measure distances from ultrasonic sensors
   measureDistances();
   
   // Check for ring boundary (highest priority)
   uint8_t boundaryDetectedSensor = 0;
   if (checkBoundary(boundaryDetectedSensor)) {
     currentState = EVADE;
     evadeStartTime = millis();
   }
   
   // Update state based on sensor readings
   updateState();
   
   // Execute current state
   switch (currentState) {
     case SEARCH:
       executeSearch();
       break;
     case TRACK:
       executeTrack();
       break;
     case ATTACK:
       executeAttack();
       break;
     case EVADE:
       executeEvade(boundaryDetectedSensor);
       break;
   }
 }
 
 // Function to update the current state based on sensor readings
 void updateState() {
   // If already in EVADE state, don't change until evade maneuver is complete
   if (currentState == EVADE && millis() - evadeStartTime < EVADE_TIME) {
     return;
   }
   
   // State transitions based on sensor readings
   if (frontDist > 0 && frontDist < ATTACK_THRESHOLD) {
     if (currentState != ATTACK) {
       attackStartTime = millis();
     }
     currentState = ATTACK;
   } 
   else if ((leftDist > 0 && leftDist < TRACK_THRESHOLD) || 
            (rightDist > 0 && rightDist < TRACK_THRESHOLD) ||
            (frontDist > 0 && frontDist < TRACK_THRESHOLD)) {
     currentState = TRACK;
   }
   else if (currentState != SEARCH) {
     currentState = SEARCH;
     searchStartTime = millis();
     searchPhase = 0;
   }
 }
 
 // Function to check ring boundary using IR sensors
 bool checkBoundary(uint8_t &detectedSensor) {
   // Read IR sensor values (only front and back)
   int irFront = analogRead(IR_FRONT);
   int irBack = analogRead(IR_BACK);
   
   // Check which IR sensor detects boundary (white line)
   if (irFront > BOUNDARY_THRESHOLD) {
     detectedSensor = IR_FRONT;
     return true;
   } 
   else if (irBack > BOUNDARY_THRESHOLD) {
     detectedSensor = IR_BACK;
     return true;
   }
   
   return false;
 }
 
 // Function to trigger ultrasonic sensor measurements
 void measureDistances() {
   // Trigger each sensor in sequence with short delay to prevent interference
   unsigned long currentTime = millis();
   
   if (currentTime - lastTriggerTime > 30) {  // 30ms cycle time for measurements
     lastTriggerTime = currentTime;
     
     // Reset received flags
     FReceived = false;
     LReceived = false;
     RReceived = false;
     
     // Trigger sensors in sequence
     switch (sensorIndex) {
       case 0:  // Left sensor
         triggerSensor(0);
         sensorIndex = 1;
         break;
       case 1:  // Front sensor
         triggerSensor(1);
         sensorIndex = 2;
         break;
       case 2:  // Right sensor
         triggerSensor(2);
         sensorIndex = 0;
         break;
     }
     
     // Convert durations to distances
     if (FReceived) {
       frontDist = FDuration * 0.034 / 2;  // Speed of sound / 2 (round trip)
       if (frontDist < ut_min || frontDist > ut_max) frontDist = 0;
     }
     
     if (LReceived) {
       leftDist = LDuration * 0.034 / 2;
       if (leftDist < ut_min || leftDist > ut_max) leftDist = 0;
     }
     
     if (RReceived) {
       rightDist = RDuration * 0.034 / 2;
       if (rightDist < ut_min || rightDist > ut_max) rightDist = 0;
     }
   }
 }
 
 // Function to trigger a specific ultrasonic sensor
 void triggerSensor(int sensor) {
   // Determine which sensor to trigger
   uint8_t trigPin;
   
   switch (sensor) {
     case 0:  // Left
       trigPin = L_trig;
       break;
     case 1:  // Front
       trigPin = F_trig;
       break;
     case 2:  // Right
       trigPin = R_trig;
       break;
     default:
       return;
   }
   
   // Send trigger pulse
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
 }
 
 // Interrupt Service Routines for ultrasonic sensors
 void R_echoISR() {
   static unsigned long startTime = 0;
   
   if (digitalRead(R_echo) == HIGH) {
     startTime = micros();
   } else {
     RDuration = micros() - startTime;
     RReceived = true;
   }
 }
 
 void F_echoISR() {
   static unsigned long startTime = 0;
   
   if (digitalRead(F_echo) == HIGH) {
     startTime = micros();
   } else {
     FDuration = micros() - startTime;
     FReceived = true;
   }
 }
 
 void L_echoISR() {
   static unsigned long startTime = 0;
   
   if (digitalRead(L_echo) == HIGH) {
     startTime = micros();
   } else {
     LDuration = micros() - startTime;
     LReceived = true;
   }
 }
 
 // Execute search mode - look for opponent
 void executeSearch() {
   unsigned long currentTime = millis();
   unsigned long searchTime = currentTime - searchStartTime;
   
   // Change search pattern periodically to increase chances of finding opponent
   if (currentTime - lastSearchPatternChange > 3000) {  // Change pattern every 3 seconds
     searchDirection = !searchDirection;
     lastSearchPatternChange = currentTime;
   }
   
   // Search pattern: aggressive spin with occasional direction changes
   if (searchDirection) {
     // Spin right
     setMotorSpeeds(maxSpeed, -maxSpeed/2);
   } else {
     // Spin left
     setMotorSpeeds(-maxSpeed/2, maxSpeed);
   }
 }
 
 // Execute track mode - turn toward opponent
 void executeTrack() {
   // Determine opponent position and turn accordingly
   if (leftDist > 0 && leftDist < TRACK_THRESHOLD && (rightDist == 0 || leftDist < rightDist)) {
     // Opponent on the left => Turn left aggressively
     setMotorSpeeds(-maxSpeed, maxSpeed);
   } 
   else if (rightDist > 0 && rightDist < TRACK_THRESHOLD && (leftDist == 0 || rightDist < leftDist)) {
     // Opponent on the right => Turn right aggressively
     setMotorSpeeds(maxSpeed, -maxSpeed);
   }
   else if (frontDist > 0 && frontDist < TRACK_THRESHOLD) {
     // Opponent in front but not close enough to attack => Move forward
     setMotorSpeeds(maxSpeed, maxSpeed);
   }
 }
 
 // Execute attack mode - charge at opponent
 void executeAttack() {
   // Full speed ahead for ramming
   setMotorSpeeds(maxSpeed, maxSpeed);
   
   // If in attack mode for too long without new sensor readings, revert to search
   if (millis() - attackStartTime > 2000 && frontDist > ATTACK_THRESHOLD) {
     currentState = SEARCH;
     searchStartTime = millis();
   }
 }
 
 // Execute evade mode - get away from boundary
 void executeEvade(uint8_t detectedSensor) {
   // Determine which sensor detected the boundary and react accordingly
   if (detectedSensor == IR_FRONT) {
     // Front sensor detected boundary: move backward
     setMotorSpeeds(-maxSpeed, -maxSpeed);
   } 
   else if (detectedSensor == IR_BACK) {
     // Back sensor detected boundary: move forward
     setMotorSpeeds(maxSpeed, maxSpeed);
   }
   else {
     // No specific sensor data or indeterminate: back up and spin
     setMotorSpeeds(-maxSpeed, -maxSpeed/2);
   }
   
   // Return to SEARCH after evade time complete
   if (millis() - evadeStartTime >= EVADE_TIME) {
     currentState = SEARCH;
     searchStartTime = millis();
   }
 }
 
 // Set motor speeds with direction control
 void setMotorSpeeds(int leftSpeed, int rightSpeed) {
   // Left motor
   if (leftSpeed >= 0) {
     digitalWrite(leftMotorA, HIGH);
     digitalWrite(leftMotorB, LOW);
     analogWrite(leftMotorPWM, min(abs(leftSpeed), maxSpeed));
   } else {
     digitalWrite(leftMotorA, LOW);
     digitalWrite(leftMotorB, HIGH);
     analogWrite(leftMotorPWM, min(abs(leftSpeed), maxSpeed));
   }
   
   // Right motor
   if (rightSpeed >= 0) {
     digitalWrite(rightMotorA, HIGH);
     digitalWrite(rightMotorB, LOW);
     analogWrite(rightMotorPWM, min(abs(rightSpeed), maxSpeed));
   } else {
     digitalWrite(rightMotorA, LOW);
     digitalWrite(rightMotorB, HIGH);
     analogWrite(rightMotorPWM, min(abs(rightSpeed), maxSpeed));
   }
 }
