#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

// Arduino variables
int ledPin = 9;
int led2Pin = 10;
int delayTime = 50;

// Serial variables
char value;
String values;
String stringValues;
int zerosLeft;
String zeros;
int length;
boolean shouldSend;

void setup() {
  // Comm
  pinMode(ledPin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  Serial.begin(9600);
  
  // Threshold
  digitalWrite(ledPin, HIGH);
  delay(delayTime);
  digitalWrite(ledPin, LOW);
}

void loop() {
  while (Serial.available()) {
    // Get input
    value = Serial.read();
    
    // Convert ASCII to binary
    zerosLeft = 8 - String(value, BIN).length();
    for (int i = 0; i < zerosLeft; i++) {
      zeros = zeros + "0";
    }
    stringValues = stringValues + String(value);
    
    // Build string
    values = values + zeros + String(value, BIN);
    
    // Loop
    zeros = "";
    delay(1);
  }
  
  if (values != "") {
    // Print ASCII and binary
    Serial.print(stringValues);
    Serial.print(" = ");
    Serial.println(values);
    length = values.length();
    shouldSend = true;
  }
  
  if (shouldSend) {
    // OOK modulation
    for (int i = 0; i < length; i++) {
      digitalWrite(led2Pin, HIGH);
      if (i != length - 1) {
        if (values.charAt(i) == '0') {
          digitalWrite(ledPin, LOW);
          delay(delayTime);
        }
        else if (values.charAt(i) == '1') {
          digitalWrite(ledPin, HIGH);
          delay(delayTime);
        }
      }
      else {
        // Turn off at the end
        if (values.charAt(i) == '0') {
          digitalWrite(ledPin, LOW);
          delay(delayTime);
        }
        else if (values.charAt(i) == '1') {
          digitalWrite(ledPin, HIGH);
          delay(delayTime);
          digitalWrite(ledPin, LOW);
        }
      }      
    }
    
    // Reset variables
    values = "";
    stringValues = "";
    digitalWrite(led2Pin, LOW);
    shouldSend = false;
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}