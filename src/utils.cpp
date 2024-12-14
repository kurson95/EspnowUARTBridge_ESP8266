#include "utils.h"

void printMacAddress(uint8_t *mac) // converts uint8_t mac addres for more readable format
{
  Serial.print("RCV MAC ADDR: ");
  for (int i = 0; i < 6; i++)
  {
    Serial.printf("%02X", mac[i]);
    if (i < 5)
      Serial.print(":");
  }
  Serial.println();
}

bool charToMAC(const char *macString, uint8_t *macArray) // converts human readable mac address to format readable for ESP
{
  if (strlen(macString) != 17)
    return false;
  for (int i = 0; i < 6; i++)
  {
    macArray[i] = strtol(macString + (i * 3), NULL, 16);
  }
  return true;
}
bool compareMACs(const uint8_t *mac1, const uint8_t *mac2) // compares 2 mac addresses
{
  return memcmp(mac1, mac2, 6) == 0;
}
void saveMacAddress(uint8_t *newAddress) // saves mac to flash
{
  preferences.putBytes("peerMac", newAddress, 6);
  // preferences.end();
  Serial.println("MAC ADDR SAVED:");
  printMacAddress(newAddress);
}
bool isValidMAC(const String &macString) // checks , if mac is valid
{
  if (macString.length() != 17)
    return false;

  for (int i = 0; i < 17; i++)
  {
    if (i % 3 == 2)
    {
      if (macString[i] != ':')
        return false; // Sprawdź separator
    }
    else
    {
      if (!isxdigit(macString[i]))
        return false; // Sprawdź cyfry szesnastkowe
    }
  }
  return true;
}
bool stringToMAC(String macString, uint8_t *macArray) // converts mac from serial to valid format
{
  if (macString.length() != 17)
    return false; // Sprawdź długość
  for (int i = 0; i < 6; i++)
  {
    // Wyciągnij każdą parę znaków i skonwertuj na liczbę szesnastkową
    macArray[i] = strtol(macString.substring(i * 3, i * 3 + 2).c_str(), NULL, 16);
  }
  return true;
}
void sendMsg(uint8_t *mac, uint8_t *data, msgtype type) // for sending data
{
  outmsg.type = type;
  memcpy(&outmsg.msg, data, BUFFER_SIZE);
  memcpy(&outmsg.mac, macaddr, 32);

#ifdef ESP32
  esp_now_send(mac, (uint8_t *)&outmsg, sizeof(outmsg));
  Serial.printf("%d BYTES SEND\n", sizeof(outmsg));
  // Resetowanie rozmiaru bufora
  buf_size = 0;
  memset(buf_send, 0, sizeof(buf_send));
  // delete[] buf_send;
  digitalWrite(LED_BUILTIN, HIGH);

#elif defined(ESP8266)
  esp_now_send(mac, (uint8_t *)&outmsg, sizeof(outmsg));
  Serial.printf("%d BYTES SEND\n", sizeof(outmsg));
  // Resetowanie rozmiaru bufora
  buf_size = 0;
  memset(buf_send, 0, sizeof(buf_send));
  // delete[] buf_send;
  digitalWrite(LED_BUILTIN, LOW);
#endif
}
void bridgeLoop() // handles serial input
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n'); // Odczyt danych do końca linii
    input.trim();                                // Usunięcie białych znaków
    Serial.println("INPUT >> " + input);
    int spaceIndex = input.indexOf(' ');
    String commandStr = (spaceIndex == -1) ? input : input.substring(0, spaceIndex);
    String arguments = (spaceIndex == -1) ? "" : input.substring(spaceIndex + 1);
    command cmd = handleCommands(commandStr);
    switch (cmd)
    {
    case ADDRECV:
      addrecv(arguments);
      break;
    case RESRECV:
      resrecv();
      break;
    case SETBR:
      setbr(arguments);
      break;
    case RST:
      reset();
      break;
    case HELP:
      Serial.println("ADDRECV FF:FF:FF:FF:FF:FF - Allow to specify mac address of receiver.\nRESRECV - Resets receiver address to default (broadcast mode).\nRST - reboot ESP.\n? - help.\nINFO - Show device info.\nREJECTUNPAIRED true/false - accept or reject data from unknown peer.\nSETBR - Set baud rate.\nAUTORST true/false - turn off or on auto reset after changing settings\n");
      break;
    case REJECTUNPAIRED:
      if (arguments == "true" || arguments == "1")
      {
        privmode(1);
      }
      else if (arguments == "false" || arguments == "0")
      {
        privmode(0);
      }
      else
      {
        Serial.println("ERROR");
      }
      break;
    case INFO:
      printinfo();

      break;
    case AUTORST:
      autoreset(arguments);
      break;
    default:
      input.getBytes(buf_send, BUFFER_SIZE);
      buf_size = input.length();
      send_timeout = micros() + timeout_micros;
      break;
    }
  }

#ifdef ESP32
  digitalWrite(LED_BUILTIN, LOW);
#elif defined(ESP8266)
  digitalWrite(LED_BUILTIN, HIGH);
#endif

  if (buf_size > 0 && (buf_size == BUFFER_SIZE || micros() >= send_timeout))
  {
    sendMsg(broadcastAddress, buf_send, DATA);
  }
}
command handleCommands(const String &com)
{

  for (int i = 0; i < commandCount; i++)
  {
    if (com == commandString[i])
    {
      return static_cast<command>(i); // Return the matching command enum
    }
  }
  return static_cast<command>(-1); // Return an invalid enum value for unknown commands
}
bool isBaudRateAllowed(long baudRate)
{
  for (long allowedRate : allowedBaudRates)
  {
    if (baudRate == allowedRate)
    {
      return true;
    }
  }
  return false;
}
void setBaudRate(long baud)
{

  if (isBaudRateAllowed(baud))
  {
    preferences.begin("setupSerial", false);
    preferences.putLong("baudRate", baud);
    Serial.write("BAUDE RATE SET: %l\n", baud);
    preferences.end();
    reset();
  }
  else
  {
    Serial.println("ERROR");
  }
}
void exprecovery()
{
#ifdef ESP32
  esp_reset_reason_t rebootReason = esp_reset_reason();
  switch (rebootReason)

  {
  case ESP_RST_PANIC:
    setBaudRate(9600);
    reset();
    break;

  default:
    break;
  }

#elif defined(ESP8266)
  String rebootReason = ESP.getResetReason();
  if (rebootReason == "Exception")
  {
    setBaudRate(9600);
    reset();
  }
#endif
}
void setupSerial()
{
  preferences.begin("setupSerial", false);
  if (!preferences.isKey("baudRate"))
  {
    preferences.putLong("baudRate", 9600);
    Serial.println("BAUD RATE SET: 9600");
    preferences.end();
  }
  preferences.begin("setupSerial", true);
  BAUD_RATE = preferences.getLong("baudRate");
  preferences.end();
  Serial.begin(BAUD_RATE);
}
void setupResetPolicy()
{
  preferences.begin("autoReset", false);

  if (!preferences.isKey("autoResetState"))
  {
    preferences.putBool("autoResetState", true);
  }
  bool resetState = preferences.getBool("autoResetState");
  autoresetena = resetState;
  preferences.end();
}
#ifdef OLED
void init_oled()
{
  display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

  // OLED used nonstandard SDA and SCL pins
  Wire.begin(D5, D6);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    return;
  }
}
void handle_oled(String text)
{
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("MAC: ");
  display->println(macaddr);
  display->setCursor(0, 17);
  display->println("RECV MAC: ");
  for (int i = 0; i < 6; i++)
  {
    display->printf("%02X", broadcastAddress[i]);
    if (i < 5)
      display->print(":");
  }
  display->setCursor(0, 35);
  display->print("LAST MSG: ");
  display->println(text);
  display->display();
}
#endif