#ifndef COMMANDS_H
#define COMMANDS_H
#include "utils.h"
#include "config.h"
#include "espnowInit.h"
extern void reset();
extern void addrecv(String input);
extern void privmode(bool mode);
extern void printinfo();
extern void resrecv();
extern void setbr(String input);
extern void autoreset(String input);
#endif