#include "commands.h"
void reset()
{

  Serial.println("REBOOTING...");
  delay(2E3);
#ifdef ESP32
  ESP.restart();
#elif defined(ESP8266)
  ESP.reset();
#endif
}
void addrecv(String input)
{
  preferences.begin("espnow", false);
  String macString = input /*.substring(8)*/;
  uint8_t newmac[6];
  if (isValidMAC(macString) && stringToMAC(macString, newmac))
  {
    Serial.println("ADD PEER OK");
    saveMacAddress(newmac);
    preferences.putBool("privmode", true);
    Serial.println("REJECT UNKNOWN: ON");
    preferences.end();
    if (autoresetena == true)
    {
      reset();
    }
  }
  else if (macString.length() <= 1)
  {
    Serial.print("NO PARAMS, ADDING LAST CONN PEER");
    saveMacAddress(LastConnAddress);
    preferences.putBool("privmode", true);
    Serial.println("REJECT UNKNOWN: ON");
    preferences.end();

     if (autoresetena == true)
    {
      reset();
    }
  }
  else
  {
    Serial.println("MAC NOK");
    preferences.end();
  }
}
void privmode(bool mode)
{
  preferences.begin("espnow", false);
  if (mode)
  {
    preferences.putBool("privmode", true);
    Serial.println("REJECT UNKNOWN: ON");
    preferences.end();
 if (autoresetena == true)
    {
      reset();
    }  }
  else
  {
    preferences.putBool("privmode", false);
    Serial.println("REJECT UNKNOWN: OFF");
    preferences.end();
 if (autoresetena == true)
    {
      reset();
    }  }
}
void printinfo()
{

#ifdef ESP32
  String id = "ESP32";
  String sdk = ESP.getSdkVersion();
#elif defined(ESP8266)
  String id = "ESP8266";
  String sdk = ESP.getFullVersion();
#endif
  preferences.begin("espnow", true);
  uint8_t buf[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  preferences.getBytes("peerMac", buf, 32);
  memcpy(broadcastAddress, buf, sizeof(broadcastAddress));
  privmodeENA = preferences.getBool("privmode", false);
  Serial.println("ESPNow UART bridge version 1.0");
  Serial.print("LAST REBOOT REASON: ");
#ifdef ESP32
  Serial.println(esp_reset_reason());
#elif defined(ESP8266)
  Serial.println(ESP.getResetReason());
#endif
  Serial.print("BAUD RATE: ");
  Serial.println(BAUD_RATE);
  Serial.print("AUTO RST: ");
  Serial.println(autoresetena);
  Serial.print("SDK: ");
  Serial.println(sdk);
  Serial.print("CHIP: ");
  Serial.println(id);
  Serial.print("REJECT UNKNOWN: ");
  Serial.println(preferences.getBool("privmode"));
  Serial.print("CPUFREQ: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println("Mhz");
  Serial.print("FLASH SIZE: ");
  Serial.print(ESP.getFlashChipSize() / 1E6);
  Serial.println("Mb");
  mac.toCharArray(macaddr, 32); // Użyj 32 dla adresu MAC
  Serial.print("MAC: ");
  Serial.println(macaddr);
  printMacAddress(broadcastAddress);
  preferences.end();
}
void resrecv()
{
  preferences.begin("espnow", false);
  saveMacAddress(defaultAddress);
  preferences.putBool("privmode", false);
  preferences.end();
  Serial.println("REJECT UNKNOWN: OFF");
 if (autoresetena == true)
    {
      reset();
    }}
void setbr(String input)
{
  long baud = input /*.substring(6)*/.toInt();
  setBaudRate(baud);
}
void autoreset(String input)
{
  preferences.begin("autoReset", false);
  if (input == "true" || input == "1")
  {
    preferences.putBool("autoResetState", true);
    preferences.end();
    reset();
  }
  else if (input == "false" || input == "0")
  {
    preferences.putBool("autoResetState", false);
    preferences.end();
    reset();

  }
  else
  {
    Serial.println("ERROR");
  }
}
