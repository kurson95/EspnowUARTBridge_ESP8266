#ifndef UTILS_H
#define UTILS_H
#include "config.h"
#include "commands.h"
extern void printMacAddress(uint8_t *mac);
extern bool charToMAC(const char *macString, uint8_t *macArray) ;
extern bool compareMACs(const uint8_t *mac1, const uint8_t *mac2) ;
extern void saveMacAddress(uint8_t *newAddress) ;
extern bool isValidMAC(const String &macString);
extern bool stringToMAC(String macString, uint8_t *macArray) ;
extern void sendMsg(uint8_t *mac,uint8_t*data,msgtype type);
extern void bridgeLoop();
extern bool isBaudRateAllowed(long baudRate);
extern void setBaudRate(long baud);
extern void exprecovery();
extern void setupSerial();
extern command handleCommands(const String &com);
extern void setupResetPolicy();
#ifdef OLED
extern void init_oled();
extern void handle_oled();
#endif
#endif