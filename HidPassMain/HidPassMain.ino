#include <FastLED.h>
#include "USB.h"
#include <USBHIDKeyboard.h>
#include "pass.h"

// Configure as Device Slave for espNow

#include <esp_now.h>
#include <WiFi.h>

#define KEY_DEBOUNCE 100
#define KEY_WAIT 500
// How many leds in your strip?
#define NUM_LEDS 1
// Define data pin for WS2812 RGB LED status diode
#define DATA_PIN 33
// Define Brightness in %
#define BRT_LED 35

// Define channel for espNow
#define CHANNEL 10


// Define the array of leds
CRGB leds[NUM_LEDS];

USBHIDKeyboard Keyboard;

extern char password[];
extern uint pass_size; 

bool locked = false; 

void lockPC() {
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_DELETE);
  delay(KEY_DEBOUNCE);
  Keyboard.releaseAll();
  delay(KEY_WAIT);
  
  Keyboard.press(KEY_RETURN);
  delay(KEY_DEBOUNCE);
  Keyboard.releaseAll();
}

void unlockPC (){
  Keyboard.press(KEY_RETURN);
  delay(KEY_DEBOUNCE);
  Keyboard.releaseAll();
  delay(KEY_WAIT);
  for(int i = 0; i < pass_size; i++){
    Keyboard.press(password[i]);
    delay(KEY_DEBOUNCE);
    Keyboard.releaseAll();
  }
  delay(KEY_WAIT);
  Keyboard.press(KEY_RETURN);
  delay(KEY_DEBOUNCE);
  Keyboard.releaseAll();
}

// Init ESP Now with fallback
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    Serial.print("AP CHANNEL "); Serial.println(WiFi.channel());
  }
}

// callback when data is recv from Master
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  Serial.println("Data received");
  Serial.println(data[0]);
  if (data[0]==0 && locked==true){
    locked = false;
    unlockPC();
    Serial.println("Unlock");
  }
    
  if (data[0]==1 && locked==false){
    locked = true;
    lockPC();
    Serial.println("Lock");
  }
 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Scanning...");

  // Setup led status diode
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical
  FastLED.setBrightness(BRT_LED);

  leds[0] = CRGB::Red;
  FastLED.show();
  delay(500);

  // Start HID
  Keyboard.begin();
  USB.begin();

  // Setup completed
  leds[0] = CRGB::Blue;
  FastLED.show();
  delay(500);

  WiFi.mode(WIFI_AP);
  configDeviceAP();
  InitESPNow();
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (locked){
    leds[0] = CRGB::Red;
  }
  else {
    leds[0] = CRGB::Green;
  }
  FastLED.show();
  delay(100);

}
