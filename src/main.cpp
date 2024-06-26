#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#include <array>

#include "data_transceiver.hpp"
#include "motor.hpp"
#include "motor_control.hpp"
#include "mpu6050.hpp"
#include "r2d2_debug_macros.hpp"
#include "steer_control.hpp"
#include "sub_control.hpp"
#include "travel_control.hpp"
#include "thermo_sensor.hpp"

 // Initialize the ThermoSensor object with the pin number connected to the OneWire bus
sen::ThermoSensor tempSensor(16);

void setup() {
    Serial.begin(115200);
    Serial.println("Temperature Sensor Reading Example");
}

void loop() {
    // Read temperature
    float temperature = tempSensor.getTemp();
    
    // Print the temperature to the Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    
    // Wait for 2 seconds before reading again
    delay(500);
}