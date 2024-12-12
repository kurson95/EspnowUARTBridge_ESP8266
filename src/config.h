#ifndef CONFIG_H
#define CONFIG_H

#include <Preferences.h>
#include <Arduino.h>
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
#define TXpin 17
#define RXpin 16
#elif defined(ESP8266)
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
enum command{ADDRECV,RESRECV,SETBR,RST,HELP,REJECTUNPAIRED,INFO,SETCHAN};
extern String commandString[];
extern const byte commandCount;
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
