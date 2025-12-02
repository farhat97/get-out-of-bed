#include <Arduino.h>

void runLedTest();
void runAlarmTest();

void setup() 
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Arduino Uno Serial Log Enabled!"); 

  pinMode(8, OUTPUT);

  Serial.println("Pin 8 set as an output");
}

void loop() 
{
  // runLedTest();
  runAlarmTest();
}

void runLedTest()
{
  // Circuit: digital pin #8 to anode (long leg) via resistor and GND to cathode (short leg)
  digitalWrite(8, HIGH);
  delay(1000);
  digitalWrite(8, LOW);
  delay(1000);
}

void runAlarmTest()
{
  tone(8, 2000);
  delay(200);
  noTone(8);
  delay(200);
}
