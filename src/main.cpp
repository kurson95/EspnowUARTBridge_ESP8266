
#include "config.h"     //global variables
#include "commands.h"   //builtin commads
#include "utils.h"      //helpers
#include "espnowInit.h" //wifi & espnow initialization
// SoftwareSerial Serial2  = SoftwareSerial(RXpin,TXpin);

void setup()
{
 
#ifdef OLED
  init_oled();
#endif
  setupSerial();
  setupResetPolicy();
  Serial1.begin(BAUD_RATE);
  pinMode(RXpin, INPUT);
  pinMode(TXpin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  // setupWiFi(); // setup wifi
  Serial.println("? HELP");
  espnowinit(); // espnow related stuff

  printinfo(); // show info about board
}

void loop()
{
  bridgeLoop(); // main program loop
                /*Place your custom code HERE
                 */
#ifdef OLED
  handle_oled();

#endif
}
