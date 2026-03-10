#include <M5NanoC6.h>
#include <Adafruit_NeoPixel.h>
#include <esp_now.h>

#include "esp_wifi.h"
#include <WiFi.h>

#define LED_PIN    20
#define ENABLE_PIN 19
#define NUM_LEDS   1

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint8_t receiverAddress[] = {0xA0, 0x76, 0x4E, 0x7A, 0x01, 0x04};

typedef struct struct_message 
{
  char action[32];
  int value;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void onDataSent(const wifi_tx_info_t *mac_addr, esp_now_send_status_t status);

void setup() 
{
  NanoC6.begin();

  
  // Set WiFi to station mode (needed for ESP-NOW)
  WiFi.mode(WIFI_STA);
  Serial.println("WiFi STA mode set");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("ESP-NOW init failed");
    while(1) delay(1000);
  }
  Serial.println("ESP-NOW initialized");

  // Register send callback
  if (esp_now_register_send_cb(onDataSent) != ESP_OK) 
  {
    Serial.println("Failed to register send callback");
    while(1) delay(1000);
  }
  Serial.println("Callback registered");

  esp_wifi_set_channel(6, WIFI_SECOND_CHAN_NONE);

  // Add peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 6;
  peerInfo.encrypt = false;
  peerInfo.ifidx = WIFI_IF_STA;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    Serial.println("Failed to add peer");
    while(1) delay(1000);
  }
  Serial.println("Peer added - Ready!");


  // ---- //
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
    Serial.println("Button pressed - sending...");
    
    strcpy(myData.action, "button_pressed");
    myData.value = 1;
    
    esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&myData, sizeof(myData));
    Serial.println("result = " + result);
    Serial.println(result == ESP_OK ? "Sent OK" : "Send failed");
  }
  if (NanoC6.BtnA.wasHold()) 
  {
    Serial.println("Button A was hold");
  }
  if (NanoC6.BtnA.wasReleased()) 
  {
    Serial.println("Button A was released");
  }

  // Set color to blue
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();

  delay(10);
}

void onDataSent(const wifi_tx_info_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}
