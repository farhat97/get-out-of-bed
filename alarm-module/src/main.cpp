#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <WiFi.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // No reset pin used
#define SCREEN_ADDRESS 0x3C // Default I2C address for SSD1306
 
RTC_DS3231 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


WiFiUDP ntpUDP;

void connectToNetwork();

// Used for debugging purposes
void scanForNetworks();

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.setDebugOutput(true);

  // Initialize Screen
  Wire.begin(8, 9);  // SDA = GPIO8, SCL = GPIO9

  // Start display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  
  // Connect to network
  connectToNetwork();
  delay(5000);

  // Change screen text 
  display.clearDisplay();
 
  // Draw simple text
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("Screen test"));
  display.display();
}
void loop() 
{
  // scanForNetworks();

  // Output some clock or something
  // DateTime now = rtc.now();
  // char *timeDisplay = now.hour() + " : " now.minute() + " : " now.second();
 
  // display.println(F(timeDisplay));
  // display.display();
  // delay(3500);
}
 
void connectToNetwork()
{

  const char* ssid = "redacted";
  const char* password = "redacted";

  WiFi.mode(WIFI_STA);
  
  Serial.println(F("Starting network connection"));
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void scanForNetworks()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(10000);
}
