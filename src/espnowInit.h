#ifndef ESPNOWINIT_H
#define ESPNOWINIT_H
#include "config.h"
#include "commands.h"
#include "utils.h"

#ifdef ESP32
#include <WiFi.h>
#include <esp_now.h>
extern esp_now_peer_info_t peerInfo;
extern void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
extern void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <espnow.h>
extern void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus);
extern void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len);
#endif
extern const char* PRIMARY_MASTER_KEY;
extern const char* LOCAL_MASTER_KEY;;
extern void setupWiFi();
extern void espnowinit();
#endif