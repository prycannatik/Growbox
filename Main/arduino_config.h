#ifndef ARDUINO_CONFIG_H
#define ARDUINO_CONFIG_H

// Pin definitions
const int PH_PIN = A0;
const int PH_POWER = 5;
const int EC_PIN = A1;
const int EC_POWER = 6;

const int LIGHT_RELAY_PIN = 7;             
const int PUMP_DUNGER_PIN = 8;             
const int PUMP_WATER_PIN = 9;              
const int PUMP_ACID_PIN = 10;              
const int PH_CALIBRATION_BUTTON_PIN = 11;  
const int PUMP_BUTTON_PIN = 13;       

// Target values for lettuce
const float TARGET_EC = 2.60;  // in mS/cm
const float TARGET_PH_LOW = 5.0;
const float TARGET_PH_HIGH = 6.2;

// PH Calibration Offset
float phCalibrationOffset = 0;
float currentPhValue = 7;
float ph7ReadValue = 665;
float ph4ReadValue = 765;


// Light cycle in hours
const long LIGHT_ON_HOURS = 16;
const long LIGHT_OFF_HOURS = 8;
unsigned long lightChangeMillis = 0;
bool lightState = false;

//Button States
bool lastPhCalibrationButtonState = digitalRead(PH_CALIBRATION_BUTTON_PIN);
bool currentPhCalibrationButtonState = digitalRead(PH_CALIBRATION_BUTTON_PIN);

bool pumpsInactiveState = digitalRead(PUMP_BUTTON_PIN);
bool currentPumpState = false;

#endif
