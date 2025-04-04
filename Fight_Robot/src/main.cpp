

#include "DashioNano33BLE.h"
#include <arduino-timer.h>
#include "motors.h"

// Configuration constants
const int MAX_SPEED = 255;
const int MIN_SPEED = 0;
const int MAX_WEAPON_SPEED = 180; // Maximum servo angle
const int MIN_WEAPON_SPEED = 0;   // Minimum servo angle
const unsigned long ANIMATION_INTERVAL = 50;
const unsigned long BUTTON_ACTION_DURATION = 200; // Duration for momentary actions

// Global variables
int speed = 0;
int targetSpeed = 0;
int weaponSpeed = 0;      // Current weapon speed/angle
int targetWeaponSpeed = 0; // Target weapon speed/angle
unsigned long lastAnimationTime = 0;
unsigned long lastWeaponChange = 0;  // Time when weapon speed was last changed
unsigned long lastDriveChange = 0;   // Time when drive speed was last changed

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
    bool updated = false;
    
    // Update drive speed if needed
    if (speed != targetSpeed) {
        if (currentTime - lastAnimationTime >= ANIMATION_INTERVAL) {
            // Interpolate speed
            if (speed < targetSpeed) {
                speed = min(speed + 5, targetSpeed);
            } else {
                speed = max(speed - 5, targetSpeed);
            }
            
            updated = true;
        }
    }
    
    // Update weapon speed if needed
    if (weaponSpeed != targetWeaponSpeed) {
        if (currentTime - lastAnimationTime >= ANIMATION_INTERVAL) {
            // Interpolate weapon speed
            if (weaponSpeed < targetWeaponSpeed) {
                weaponSpeed = min(weaponSpeed + 5, targetWeaponSpeed);
            } else {
                weaponSpeed = max(weaponSpeed - 5, targetWeaponSpeed);
            }
            
            // Update servo position
            motors.BLDCMotorControl(weaponSpeed);
            
            updated = true;
        }
    }
    
    if (updated) {
        // Decide which value to display on the dial
        // Show the most recently changed value
        int displayValue = (lastWeaponChange > lastDriveChange) ? weaponSpeed : speed;
        
        // Update dial with current interpolated value
        String message = dashDevice.getDialMessage("D01", displayValue);
        ble_con.sendMessage(message);
        
        lastAnimationTime = currentTime;
        return true; // Continue animation
    }
    
    return (speed != targetSpeed) || (weaponSpeed != targetWeaponSpeed); // Continue if either needs updating
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
    message += dashDevice.getSliderMessage("SL2", weaponSpeed);
    message += dashDevice.getDialMessage("D01", (lastWeaponChange > lastDriveChange) ? weaponSpeed : speed);
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
                
                // Update timestamp for drive speed change
                lastDriveChange = millis();
                
                // Update dial message
                String message = dashDevice.getDialMessage("D01", baseSpeed);
                ble_con.sendMessage(message);
                
                // Start speed interpolation animation
                timer.every(ANIMATION_INTERVAL, [](void*) -> bool {
                    return updateSpeedAnimation(dashDevice, ble_con);
                });
            }
            else if(MessageData->idStr == "SL2") {
                // Set weapon speed
                targetWeaponSpeed = constrain(MessageData->payloadStr.toFloat(), MIN_WEAPON_SPEED, MAX_WEAPON_SPEED);
                
                // Update timestamp for weapon speed change
                lastWeaponChange = millis();
                
                // Update dial message to show weapon speed
                String message = dashDevice.getDialMessage("D01", targetWeaponSpeed);
                ble_con.sendMessage(message);
                
                // Start speed interpolation animation if not already running
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

                // Update timestamp for drive action
                lastDriveChange = millis();
                
                // Update dial to reflect speed change
                String message = dashDevice.getDialMessage("D01", backwardAdjustment);
                ble_con.sendMessage(message);
            }
            else if (MessageData->idStr == "BR1") {
                // Right turn
                turnDirection = 1;
                turnAdjustment = min(turnAdjustment + 30, baseSpeed);

                // Update timestamp for drive action
                lastDriveChange = millis();
                
                // Update dial to reflect adjustment
                String message = dashDevice.getDialMessage("D01", baseSpeed - turnAdjustment);
                ble_con.sendMessage(message);
            }
            else if (MessageData->idStr == "BL1") {
                // Left turn
                turnDirection = -1;
                turnAdjustment = min(turnAdjustment + 30, baseSpeed);

                // Update timestamp for drive action
                lastDriveChange = millis();
                
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

                // Update timestamp for drive action
                lastDriveChange = millis();
                
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
