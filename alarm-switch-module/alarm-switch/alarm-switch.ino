#include <M5NanoC6.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    20
#define ENABLE_PIN 19
#define NUM_LEDS   1

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() 
{
  NanoC6.begin();

  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, HIGH);
  strip.begin();
  strip.show();
}

void loop() 
{
  NanoC6.update();
  if (NanoC6.BtnA.wasPressed()) 
  {
    Serial.println("Button A was pressed");
  }
  if (NanoC6.BtnA.wasHold()) 
  {
    Serial.println("Button A was hold");
  }
  if (NanoC6.BtnA.wasReleased()) 
  {
    Serial.println("Button A was released");
  }

  //BLUE
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();
}