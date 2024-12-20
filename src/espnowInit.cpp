#include "espnowInit.h"
#include "config.h"
/*ESP-NOW related functions*/
#ifdef ESP32

void OnDataSent(const uint8_t *mac, esp_now_send_status_t status)
{
  Serial.print("SEND: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "OK" : " FAIL");

  if (status == 0)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }
  else
  {

    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
}
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{

  memcpy(&inmsg, incomingData, len); // Only copy `len` bytes
  if (inmsg.type == ACK)
  {
    char msg[len];
    sprintf(msg, "%s >> %s\n", inmsg.mac, inmsg.msg);
    Serial.write(msg);
  }
  else if (inmsg.type == DATA)
  {
    charToMAC(inmsg.mac, LastConnAddress);
    if (privmodeENA && compareMACs(LastConnAddress, broadcastAddress) || !privmodeENA)
    {
      // memcpy(&LastConnAddress, inmsg.mac, sizeof(inmsg.mac));

      char msg[len];
      sprintf(msg, "%s >> %s\n", inmsg.mac, inmsg.msg);
      if (!compareMACs(broadcastAddress, LastConnAddress))
      {
        memcpy(peerInfo.peer_addr, LastConnAddress, sizeof(LastConnAddress));
        esp_now_add_peer(&peerInfo);
      }
      sendMsg(LastConnAddress, (uint8_t *)"MSG ACK", ACK);
      Serial.write(msg);
      // memset(buf_recv, 0, sizeof(buf_recv));

      digitalWrite(LED_BUILTIN, LOW);
      memset(msg, 0, sizeof(msg));
    }
    if (privmodeENA && !compareMACs(LastConnAddress, broadcastAddress))
    {
      sendMsg(LastConnAddress, (uint8_t *)"MSG NACK", ACK);
    }
    // memcpy(peerInfo.peer_addr, broadcastAddress, sizeof(broadcastAddress));
    // esp_now_add_peer(&peerInfo);
  }
}
#elif defined(ESP8266)

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  Serial.print("SEND: ");
  if (sendStatus == 0)
  {
    Serial.println("OK");
    digitalWrite(LED_BUILTIN, LOW);
  }
  else
  {
    Serial.println("FAIL");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
// Function that executes on data received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len)
{

  memcpy(&inmsg, incomingData, len); // Only copy `len` bytes
  if (inmsg.type == ACK)
  {
    char msg[len];
    sprintf(msg, "%s >> %s\n", inmsg.mac, inmsg.msg);
    Serial.write(msg);
  }
  else if (inmsg.type == DATA)
  {
    charToMAC(inmsg.mac, LastConnAddress);
    if (privmodeENA && compareMACs(LastConnAddress, broadcastAddress) || !privmodeENA)
    {
      // memcpy(&LastConnAddress, inmsg.mac, sizeof(inmsg.mac));

      char msg[len];
      memcpy(&oledBuf, inmsg.msg, OLED_BUFF_SIZE);
      sprintf(msg, "%s >> %s\n", inmsg.mac, inmsg.msg);

      sendMsg(LastConnAddress, (uint8_t *)"MSG ACK", ACK);
      Serial.write(msg);
      Serial1.write(inmsg.msg);

      // memset(buf_recv, 0, sizeof(buf_recv));
      digitalWrite(LED_BUILTIN, LOW);
      memset(msg, 0, sizeof(msg));
    }
    if (privmodeENA && !compareMACs(LastConnAddress, broadcastAddress))
    {
      sendMsg(LastConnAddress, (uint8_t *)"MSG NACK", ACK);
    }
  }
}
#endif
//

void espnowinit()
{
  WiFi.mode(WIFI_AP_STA);
  mac = WiFi.macAddress();
  WiFi.begin();

  WiFi.macAddress().toCharArray(macaddr, 32); // Użyj 32 dla adresu MAC
#ifdef ESP32
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP-NOW INIT FAIL");
    return;
  }
  else
  {
    Serial.println("ESP-NOW INIT OK");
  }
#elif defined(ESP8266)
  if (esp_now_init() != 0)
  {
    Serial.println("ESP-NOW INIT FAIL");
    return;
  }
  else
  {
    Serial.println("ESP-NOW INIT OK");
  }

#endif
  preferences.begin("espnow", false);
  if(!preferences.isKey("peerMAc")){
      preferences.putBytes("peerMac", defaultAddress, 6);
  }
  if(!preferences.isKey("privmode")){
    preferences.putBool("privmode",false);
  }
  uint8_t buf[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  preferences.getBytes("peerMac", buf, 32);
  memcpy(broadcastAddress, buf, sizeof(broadcastAddress));
  preferences.end();
#ifdef ESP32
  esp_now_register_send_cb(OnDataSent);
  memset(&peerInfo, 0, sizeof(peerInfo)); // Wyzerowanie struktury
  memcpy(peerInfo.peer_addr, broadcastAddress, sizeof(broadcastAddress));
  if (!compareMACs(broadcastAddress, defaultAddress))
  {
    enc = true;
    memcpy(peerInfo.lmk, LOCAL_MASTER_KEY, 16);
    esp_now_set_pmk((uint8_t *)PRIMARY_MASTER_KEY);
    peerInfo.encrypt = true;
  }
  else
  {
    enc = false;
    peerInfo.encrypt = false;
  }
  peerInfo.channel = 1;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("PEER ADD FAIL");
    return;
  }
  else
  {
    Serial.println("PEER ADD OK");
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
#elif defined(ESP8266)
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  if (!compareMACs(broadcastAddress, defaultAddress))
  {
    enc = true;
    esp_now_set_kok((uint8_t *)PRIMARY_MASTER_KEY, 16);
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, (uint8_t *)LOCAL_MASTER_KEY, 16);
  }
  else
  {
    enc = false;
    esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
#endif
}