#ifndef CONFIG_H
#define CONFIG_H

#include <Preferences.h>
#include <Arduino.h>
#ifdef OLED 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // If not work please try 0x3D
#define OLED_SDA D5         // Stock firmware shows wrong pins
#define OLED_SCL D6         // They swap SDA with SCL ;)

extern Adafruit_SSD1306 *display;
#endif
extern Preferences preferences;
extern long BAUD_RATE;
extern const char* PRIMARY_MASTER_KEY;
extern const char* LOCAL_MASTER_KEY;
// to large BAUDE_RATE & BUFFER_SIZE variable can cause crash. Use with caution
#define BUFFER_SIZE 64
// #define BUFFER_SIZE 128
// #define BUFFER_SIZE 256
// #define BUFFER_SIZE 512
#define LED_BUILTIN 2
// for software serial
#ifdef ESP32
#include <WiFi.h>
#include <esp_now.h>
#define TXpin 17
#define RXpin 16
extern esp_now_peer_info peerInfo;

#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <espnow.h>
#define RXpin 13
#define TXpin 15
#endif
extern uint8_t broadcastAddress[6]; // receiver mac address
extern uint8_t LastConnAddress[32]; // Stores mac address of last conneted device
extern uint8_t defaultAddress[32];  // defualt address
extern char macaddr[32];            
extern uint8_t buf_send[BUFFER_SIZE]; // stores input from Serial , except commands
extern uint8_t buf_size;
extern uint32_t send_timeout;
extern bool privmodeENA; // true - reject msg from uknown peer , false - accept very msg
extern String success;
extern String mac;
extern const uint32_t timeout_micros;
extern const long allowedBaudRates[12];
enum command{ADDRECV,RESRECV,SETBR,RST,HELP,DEUNP,INFO,AUTORST};
extern String commandString[];
extern const byte commandCount;
extern bool autoresetena;
extern char oledBuf[];
extern char oledBufSend[];
extern const uint OLED_BUFF_SIZE;
extern bool enc;
enum msgtype
{
  DATA,//for data
  ACK//for acknowlege
};
/*Structure for sending ,receiving data */
typedef struct data
{
  msgtype type;   // type of msg - DATA or ACK
  char msg[BUFFER_SIZE]; // actual msg
  char mac[32];          // device MAC address
} data;
extern data inmsg;
extern data outmsg;
#endif
