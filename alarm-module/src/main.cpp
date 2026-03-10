#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <esp_now.h>
#include <esp_wifi.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  // No reset pin used
#define SCREEN_ADDRESS 0x3C // Default I2C address for SSD1306

#define ALARM_TIME "06:30:00"
 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,"pool.ntp.org", -18000, (24 * 60 * 60 * 1000));

boolean ALARM_FLAG = false;

typedef struct struct_message 
{
  char action[32];
  int value;
} struct_message;

struct_message incomingData;

void onDataRecv(const uint8_t * mac, const uint8_t *incomingBytes, int len) 
{
  memcpy(&incomingData, incomingBytes, sizeof(incomingData));
  Serial.print("Action received: ");
  Serial.print("Value: ");
  Serial.println(incomingData.value);
  
  ALARM_FLAG = true;
}

void connectToNetwork();
void startAlarm(boolean* alarmFlag);

// Used for debugging purposes
void scanForNetworks();

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.setDebugOutput(true);

  // Initialize Screen
  Wire.begin(8, 9);  // SDA = GPIO8, SCL = GPIO9

  WiFi.mode(WIFI_STA);

  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());

  // ESPNow
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("ESP-NOW init failed");
    while(1) delay(1000);
  }
  Serial.println("ESP-NOW initialized");
  if (esp_now_register_recv_cb(onDataRecv) != ESP_OK)
  {
    Serial.println("Failed to register send callback");
    while(1) delay(1000);
  }
  Serial.println("Receive callback registered");

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

  if (timeClient.getFormattedTime().equals(ALARM_TIME))
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Dale broder es hora");
    display.display();
    startAlarm(&ALARM_FLAG);
  }
}
 
void connectToNetwork()
{

  // TODO: get this from a text file
  const char* ssid = "redacted";
  const char* password = "redacted";

  Serial.println(F("Starting network connection"));
  WiFi.begin(ssid, password);

  // Log the channel
  uint8_t channel;
  wifi_second_chan_t secondChan;
  esp_wifi_get_channel(&channel, &secondChan);
  Serial.print("WiFi Channel: ");
  Serial.println(channel);

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
  display.println("Connected-"+ ipString);
  String chanStr = String(channel);
  display.println("\nChan: "+ chanStr);
  display.display();
}

void startAlarm(boolean* flag)
{
  while (!*flag)
  {
    tone(20, 1000);
    delay(200);
    noTone(20);
    delay(200);
  }

  // Reset flag
  *flag = false;
}

void scanForNetworks()
{
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) 
  {
    Serial.println("no networks found");
  } else 
  {
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
