#include <Arduino.h>

// Simple Blink program
// Circuit: digital pin #8 to anode (long leg) via resistor and GND to cathode (short leg)

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Arduino Uno Serial Log Enabled!"); 

  pinMode(8, OUTPUT);

  Serial.println("Pin 8 set as an output");
}

void loop() {
  digitalWrite(8, HIGH);
  delay(500);
  digitalWrite(8, LOW);
  delay(500);
}
