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
    if (input.startsWith("ADDRECV"))
    {
      addrecv(input);
    }
    else if (input.startsWith("RESRECV"))
    {
      resrecv();
    }
    else if (input.startsWith("RST"))
    {
      reset();
    }
    else if (input.startsWith("?"))
    {
      Serial.println("ADDRECV FF:FF:FF:FF:FF:FF - Allow to specify mac address of receiver.\nRESRECV - Resets receiver address to default (broadcast mode).\nRST - reboot ESP.\n? - help.\nINFO - Show device info.\nREJECTUNPAIRED true/false - accept or reject data from unknown peer.\nSETBR - Set baud rate.\n");
    }
    else if (input.startsWith("REJECTUNPAIRED"))
    {
      String mode = input.substring(15);
      if (mode == "true" || mode == "1")
      {
        privmode(1);
      }
      else if (mode == "false" || mode == "0")
      {
        privmode(0);
      }
      else
      {
        Serial.println("ERROR");
      }
    }
    else if (input.startsWith("INFO"))
    {
      printinfo();
    }
    else if (input.startsWith("SETBR"))
    {
      setbr(input);
    }
    else
    {
      input.getBytes(buf_send, BUFFER_SIZE);
      buf_size = input.length();
      send_timeout = micros() + timeout_micros;
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
