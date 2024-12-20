#include "commands.h"
/*This file contains command functions. */
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
    }
  }
  else
  {
    preferences.putBool("privmode", false);
    Serial.println("REJECT UNKNOWN: OFF");
    preferences.end();
    if (autoresetena == true)
    {
      reset();
    }
  }
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
  privmodeENA = preferences.getBool("privmode", false);
  Serial.println("ESPNow UART bridge version 1.0");
  Serial.print("LAST REBOOT REASON: ");
#ifdef ESP32
  int reason = esp_reset_reason();
  switch ( reason)
  {
    case 1 : Serial.println ("POWERON_RESET");break;          /**<1,  Vbat power on reset*/
    case 3 : Serial.println ("SW_RESET");break;               /**<3,  Software reset digital core*/
    case 4 : Serial.println ("OWDT_RESET");break;             /**<4,  Legacy watch dog reset digital core*/
    case 5 : Serial.println ("DEEPSLEEP_RESET");break;        /**<5,  Deep Sleep reset digital core*/
    case 6 : Serial.println ("SDIO_RESET");break;             /**<6,  Reset by SLC module, reset digital core*/
    case 7 : Serial.println ("TG0WDT_SYS_RESET");break;       /**<7,  Timer Group0 Watch dog reset digital core*/
    case 8 : Serial.println ("TG1WDT_SYS_RESET");break;       /**<8,  Timer Group1 Watch dog reset digital core*/
    case 9 : Serial.println ("RTCWDT_SYS_RESET");break;       /**<9,  RTC Watch dog Reset digital core*/
    case 10 : Serial.println ("INTRUSION_RESET");break;       /**<10, Instrusion tested to reset CPU*/
    case 11 : Serial.println ("TGWDT_CPU_RESET");break;       /**<11, Time Group reset CPU*/
    case 12 : Serial.println ("SW_CPU_RESET");break;          /**<12, Software reset CPU*/
    case 13 : Serial.println ("RTCWDT_CPU_RESET");break;      /**<13, RTC Watch dog Reset CPU*/
    case 14 : Serial.println ("EXT_CPU_RESET");break;         /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : Serial.println ("RTCWDT_BROWN_OUT_RESET");break;/**<15, Reset when the vdd voltage is not stable*/
    case 16 : Serial.println ("RTCWDT_RTC_RESET");break;      /**<16, RTC Watch dog reset digital core and rtc module*/
    default : Serial.println ("NO_MEAN");
  }
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
  Serial.print("ENCRYPTION: ");
  Serial.println(enc);
  Serial.print("CPUFREQ: ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println("Mhz");
  Serial.print("FLASH SIZE: ");
  Serial.print(ESP.getFlashChipSize() / 1E6);
  Serial.println("Mb");
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
  }
}
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
