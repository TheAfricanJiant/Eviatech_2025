/******************************************************************
* Project: Eviatech 2025 Competion/Summo                          *
* -----                                                           *
* Authors: Tambu Precious 2025                                    *                               
* -----                                                           *
* Goal: A fast Summo Wrestling Robot                              *
*******************************************************************/

#include "DashioNano33BLE.h"
#include <arduino-timer.h>
#include "motors.h"

// Configuration constants
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;
const unsigned long ANIMATION_INTERVAL = 50;
const unsigned long BUTTON_ACTION_DURATION = 200; // Duration for momentary actions

// Global variables
int speed = 0;
int targetSpeed = 0;
unsigned long lastAnimationTime = 0;

// Racing control variables
int baseSpeed = 0;        // Overall forward speed from slider
int turnAdjustment = 0;   // Turn speed adjustment
int turnDirection = 0;    // 1 for right, -1 for left, 0 for no turn
int backwardAdjustment = 0; // Backward speed subtraction
unsigned long lastButtonPressTime = 0;
bool isMomentaryAction = false;
bool isStopped = false;

// Speed animation function
bool updateSpeedAnimation(DashDevice& dashDevice, DashBLE& ble_con) {
    unsigned long currentTime = millis();
    
    if (speed != targetSpeed) {
        if (currentTime - lastAnimationTime >= ANIMATION_INTERVAL) {
            // Interpolate speed
            if (speed < targetSpeed) {
                speed = min(speed + 5, targetSpeed);
            } else {
                speed = max(speed - 5, targetSpeed);
            }
            
            // Update dial with current interpolated speed
            String message = dashDevice.getDialMessage("D01", speed);
            ble_con.sendMessage(message);
            
            lastAnimationTime = currentTime;
        }
        return true; // Continue animation
    }
    
    return false; // Stop animation when target reached
}

// Calculate motor speeds for racing control
void calculateRacingMotorSpeeds() {
    // If stopped, do not move
    if (isStopped) {
        motors.stop();
        return;
    }

    // Check if momentary action has expired
    if (isMomentaryAction && (millis() - lastButtonPressTime > BUTTON_ACTION_DURATION)) {
        // Reset momentary states
        turnDirection = 0;
        turnAdjustment = 0;
        backwardAdjustment = 0;
        isMomentaryAction = false;
    }

    // Handle backward movement first
    if (backwardAdjustment > 0) {
        // Immediate backward movement
        motors.leftMotor(backwardAdjustment, 0);
        motors.rightMotor(backwardAdjustment, 0);
        return;
    }

    // Calculate turning speeds
    int leftMotorSpeed = baseSpeed;
    int rightMotorSpeed = baseSpeed;

    // Apply turning logic
    if (turnDirection > 0) {  // Right turn
        rightMotorSpeed = max(0, baseSpeed - turnAdjustment);
    } else if (turnDirection < 0) {  // Left turn
        leftMotorSpeed = max(0, baseSpeed - turnAdjustment);
    }

    // Apply motor speeds
    motors.leftMotor(leftMotorSpeed, 1);
    motors.rightMotor(rightMotorSpeed, 1);
}

// Global instances
DashDevice dashDevice("SAMD_NINA_Type");
DashBLE ble_con(&dashDevice, true);
auto timer = timer_create_default();

void processStatus(ConnectionType ConnectionType) {
    String message;
    message.reserve(1024);
    
    message = dashDevice.getSliderMessage("SL1", speed);
    message += dashDevice.getDialMessage("D01", speed);
    message += dashDevice.getButtonMessage("BL1", "on");
    message += dashDevice.getButtonMessage("BR1", "on");
    message += dashDevice.getButtonMessage("BF1", "on");
    message += dashDevice.getButtonMessage("BB1", "on");
    message += dashDevice.getButtonMessage("BS1", "on");
    
    ble_con.sendMessage(message);
}

void processIncomingMessage(MessageData* MessageData) {
    switch(MessageData->control) {
        case status:
            processStatus(MessageData->connectionType);
            break;
        
        case slider:
            if(MessageData->idStr == "SL1") {
                // Exit stopped state if a new speed is set
                if (isStopped) {
                    isStopped = false;
                }

                // Set base speed and target speed for animation
                baseSpeed = constrain(MessageData->payloadStr.toFloat(), MIN_SPEED, MAX_SPEED);
                targetSpeed = baseSpeed;
                
                // Update dial message
                String message = dashDevice.getDialMessage("D01", baseSpeed);
                ble_con.sendMessage(message);
                
                // Start speed interpolation animation
                timer.every(ANIMATION_INTERVAL, [](void*) -> bool {
                    return updateSpeedAnimation(dashDevice, ble_con);
                });
            }
            break;
        
        case button:
            // Check if currently stopped
            if (isStopped && MessageData->idStr != "BS1") {
                // Any button other than stop will exit stopped state
                isStopped = false;
            }

            // Reset momentary action flag and last press time
            lastButtonPressTime = millis();
            isMomentaryAction = true;

            if(MessageData->idStr == "BB1") {
                // Immediate backward movement
                backwardAdjustment = baseSpeed; // Use base speed for backward
                turnDirection = 0;
                turnAdjustment = 0;

                // Update dial to reflect speed change
                String message = dashDevice.getDialMessage("D01", backwardAdjustment);
                ble_con.sendMessage(message);
            }
            else if (MessageData->idStr == "BR1") {
                // Right turn
                turnDirection = 1;
                turnAdjustment = min(turnAdjustment + 30, baseSpeed);

                // Update dial to reflect adjustment
                String message = dashDevice.getDialMessage("D01", baseSpeed - turnAdjustment);
                ble_con.sendMessage(message);
            }
            else if (MessageData->idStr == "BL1") {
                // Left turn
                turnDirection = -1;
                turnAdjustment = min(turnAdjustment + 30, baseSpeed);

                // Update dial to reflect adjustment
                String message = dashDevice.getDialMessage("D01", baseSpeed - turnAdjustment);
                ble_con.sendMessage(message);
            }
            else if (MessageData->idStr == "BS1") {
                // Persistent stop
                isStopped = true;
                motors.stop();
                turnDirection = 0;
                turnAdjustment = 0;
                backwardAdjustment = 0;
                isMomentaryAction = false;

                // Reset dial to zero
                String message = dashDevice.getDialMessage("D01", 0);
                ble_con.sendMessage(message);
            }
            else {
                // Reset all adjustments when no button is pressed
                turnDirection = 0;
                turnAdjustment = 0;
                backwardAdjustment = 0;
                isMomentaryAction = false;
            }
            break;
    }
}

void setup() {
    ble_con.setCallback(&processIncomingMessage);
    ble_con.begin();
    
    dashDevice.setup(ble_con.macAddress(), "Itel220");
    
    // Initialize motor pins
    motors.initMotorPins();
}

void loop() {
    ble_con.run();
    timer.tick(); // Non-blocking timer tick
    
    // Continuously calculate and apply racing motor speeds
    calculateRacingMotorSpeeds();
}
