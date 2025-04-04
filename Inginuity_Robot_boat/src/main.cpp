#include "DashioNano33BLE.h"
#include <arduino-timer.h>
#include "motors.h"

// Configuration constants
const int MAX_ENGINE_SPEED = 180;
const int MIN_ENGINE_SPEED = 0;
const int FIN_CENTER = 50;         // Center position for fin
const unsigned long ANIMATION_INTERVAL = 50;
const unsigned long BUTTON_PRESS_DURATION = 500; // Duration for button press

// Global variables
int engineSpeed = 0;
int targetEngineSpeed = 0;
int finPosition = FIN_CENTER;      // Start at center position
unsigned long lastAnimationTime = 0;
unsigned long lastEngineChange = 0;
unsigned long lastFinChange = 0;
unsigned long lastButtonPressTime = 0;
bool isEngineRunning = true;       // Flag for engine state
bool isButtonPressed = false;

// Animation function for smooth movement
bool updateAnimations(DashDevice& dashDevice, DashBLE& ble_con) {
    unsigned long currentTime = millis();
    bool updated = false;
    
    // Update engine speed if needed
    if (engineSpeed != targetEngineSpeed && isEngineRunning) {
        if (currentTime - lastAnimationTime >= ANIMATION_INTERVAL) {
            // Interpolate speed
            if (engineSpeed < targetEngineSpeed) {
                engineSpeed = min(engineSpeed + 5, targetEngineSpeed);
            } else {
                engineSpeed = max(engineSpeed - 5, targetEngineSpeed);
            }
            
            // Update engine servo position
            motors.BLDCMotorControl(engineSpeed);
            updated = true;
        }
    }
    
    // Check if button press has timed out and we need to return to center
    if (isButtonPressed && (currentTime - lastButtonPressTime > BUTTON_PRESS_DURATION)) {
        // Return fin to center position
        motors.Left_Right(2); // Assuming 2 is center position
        isButtonPressed = false;
        updated = true;
    }
    
    if (updated) {
        // Decide which value to display on the dial
        // For water robot, always show engine speed on the dial
        String message = dashDevice.getDialMessage("D01", engineSpeed);
        ble_con.sendMessage(message);
        
        lastAnimationTime = currentTime;
        return true; // Continue animation
    }
    
    return (engineSpeed != targetEngineSpeed) || isButtonPressed;
}

// Global instances
DashDevice dashDevice("SAMD_NINA_Type");
DashBLE ble_con(&dashDevice, true);
auto timer = timer_create_default();

void processStatus(ConnectionType ConnectionType) {
    String message;
    message.reserve(1024);
    
    message = dashDevice.getSliderMessage("SL1", engineSpeed); 
    message += dashDevice.getDialMessage("D01", engineSpeed);
    message += dashDevice.getButtonMessage("BL1", "on");  // Left fin control
    message += dashDevice.getButtonMessage("BR1", "on");  // Right fin control
    message += dashDevice.getButtonMessage("BS1", "on");  // Stop engine
    
    ble_con.sendMessage(message);
}

void processIncomingMessage(MessageData* MessageData) {
    switch(MessageData->control) {
        case status:
            processStatus(MessageData->connectionType);
            break;
        
        case slider:
            if(MessageData->idStr == "SL1") {
                // Set engine speed and target speed for animation
                targetEngineSpeed = constrain(MessageData->payloadStr.toFloat(), MIN_ENGINE_SPEED, MAX_ENGINE_SPEED);
                
                // Enable engine if it was stopped
                isEngineRunning = true;
                
                // Update timestamp for engine speed change
                lastEngineChange = millis();
                
                // Update dial message
                String message = dashDevice.getDialMessage("D01", targetEngineSpeed);
                ble_con.sendMessage(message);
                
                // Start animation if not already running
                timer.every(ANIMATION_INTERVAL, [](void*) -> bool {
                    return updateAnimations(dashDevice, ble_con);
                });
            }
            break;
        
        case button:
            if(MessageData->idStr == "BL1") {
                // Turn fin left using the Motors class
                motors.Left_Right(0); // 0 for left
                
                // Set button pressed flag and update timestamp
                isButtonPressed = true;
                lastButtonPressTime = millis();
                
                // Update timestamp for fin change
                lastFinChange = millis();
                
                // Start animation if not already running
                timer.every(ANIMATION_INTERVAL, [](void*) -> bool {
                    return updateAnimations(dashDevice, ble_con);
                });
            }
            else if (MessageData->idStr == "BR1") {
                // Turn fin right using the Motors class
                motors.Left_Right(1); // 1 for right
                
                // Set button pressed flag and update timestamp
                isButtonPressed = true;
                lastButtonPressTime = millis();
                
                // Update timestamp for fin change
                lastFinChange = millis();
                
                // Start animation if not already running
                timer.every(ANIMATION_INTERVAL, [](void*) -> bool {
                    return updateAnimations(dashDevice, ble_con);
                });
            }
            else if (MessageData->idStr == "BS1") {
                // Toggle engine on/off
                isEngineRunning = !isEngineRunning;
                
                if (!isEngineRunning) {
                    // Set engine to zero when stopped
                    engineSpeed = 0;
                    targetEngineSpeed = 0;
                    motors.BLDCMotorControl(0);
                    
                    // Update dial to zero
                    String message = dashDevice.getDialMessage("D01", 0);
                    ble_con.sendMessage(message);
                } else {
                    // Resume to previous target when restarted
                    String message = dashDevice.getDialMessage("D01", targetEngineSpeed);
                    ble_con.sendMessage(message);
                }
                
                // Update timestamp
                lastEngineChange = millis();
            }
            break;
    }
}

void setup() {
    ble_con.setCallback(&processIncomingMessage);
    ble_con.begin();
    
    dashDevice.setup(ble_con.macAddress(), "Itel220");
    
    // Initialize motor pins (which also initializes the servos)
    motors.initMotorPins();
    
    // Set initial positions
    motors.BLDCMotorControl(0);  // Set engine to zero
    motors.Left_Right(2);        // Center fin position (assuming 2 is center)
}

void loop() {
    ble_con.run();
    timer.tick(); // Non-blocking timer tick
}
