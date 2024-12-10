

#include "commands.h"   //builtin commads
#include "utils.h"      //helpers
#include "config.h"     //global variables
#include "espnowInit.h" //wifi & espnow initialization
// SoftwareSerial Serial2  = SoftwareSerial(RXpin,TXpin);
#ifdef ESP32
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include "esp_log.h"
#endif
void setup()
{
  setupSerial();
  #ifdef ESP32
  esp_log_level_set("*", ESP_LOG_ERROR);   // set all components to ERROR level
  esp_log_level_set("wifi", ESP_LOG_WARN); // enable WARN logs from WiFi stack
  esp_log_level_set("dhcpc", ESP_LOG_INFO);
  esp_log_level_set("espnow", ESP_LOG_INFO);
  #endif
  Serial1.begin(BAUD_RATE);
  pinMode(RXpin, INPUT);
  pinMode(TXpin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  setupWiFi();  // setup wifi
  espnowinit(); // espnow related stuff
  Serial.println("? HELP");
  printinfo(); // show info about board
  preferences.end();
}

void loop()
{
  bridgeLoop(); // main program loop
                /*Place your custom code HERE
                 */
}
