#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // No reset pin used
#define SCREEN_ADDRESS 0x3C // Default I2C address for SSD1306
 
RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);  // SDA = GPIO8, SCL = GPIO9

  // Start display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
 
  display.clearDisplay();

  // Draw simple text
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Screen test"));
  display.display();
}
void loop() {
  // Output some clock or something
  // DateTime now = rtc.now();
  // char *timeDisplay = now.hour() + " : " now.minute() + " : " now.second();
 
  // display.println(F(timeDisplay));
  // display.display();
  // delay(3500);
}
 
