#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // No reset pin used
#define SCREEN_ADDRESS 0x3C // Default I2C address for SSD1306

#define ALARM_TIME "15:00:00"
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org", -21600, 60000);

void connectToNetwork();
void startAlarm();

// Used for debugging purposes
void scanForNetworks();

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.setDebugOutput(true);

  // Initialize Screen
  Wire.begin(8, 9);  // SDA = GPIO8, SCL = GPIO9

  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());

  // Start display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  
  // Connect to network
  connectToNetwork();
  delay(3000);

  // Change screen text 
  display.clearDisplay();
 
  // Draw simple text
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Init Complete");
  display.display();

  timeClient.begin();

  // Output for Buzzer
  pinMode(20, OUTPUT);
}
void loop() 
{
  display.clearDisplay();
  display.setCursor(0, 0);
  timeClient.update();
  display.println(timeClient.getFormattedTime());
  display.display();

  if (timeClient.getFormattedTime().equals(ALARM_TIME)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Dale broder es hora");
    display.display();
    startAlarm();
  }
}
 
void connectToNetwork()
{

  // TODO: get this from a text file
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

  // Change screen text 
  display.clearDisplay();
 
  // Draw simple text
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  String ipString = WiFi.localIP().toString();
  display.println("Connected! IP: \n" + ipString);
  display.display();
}

void startAlarm()
{
  // TODO: remove testIndex and use external ESP32 to turn alarm off
  int testIndex = 0;
  while (testIndex < 7)
  {
    tone(20, 2000);
    delay(200);
    noTone(20);
    delay(200);
    
    testIndex++;
  }
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
