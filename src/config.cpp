#include "config.h"
Preferences preferences;
long BAUD_RATE;
uint8_t defaultAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t buf_size = 0;
uint32_t send_timeout = 0;
const uint32_t timeout_micros = (int)(1.0 / BAUD_RATE * 1E6) * 20;
uint8_t LastConnAddress[32];
uint8_t broadcastAddress[6];
char macaddr[32];
uint8_t buf_send[BUFFER_SIZE] = {0};
bool privmodeENA;
String mac;
String success;
data inmsg;
const long allowedBaudRates[] = {300, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400, 250000};
data outmsg;
