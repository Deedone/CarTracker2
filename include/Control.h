#ifndef CONTROL_H
#define CONTROL_H
#include "Storage.h"
#include "Arduino.h"


#define CMD_RESET 'R'
#define CMD_GETDIST 'G'
#define CMD_SETDIST 'S'
#define CMD_GETTRIGS 'T'
#define CMD_ADDTRIG 't'
#define CMD_DELTRIG 'd'
#define CMD_END '\n'


void processCommands(Storage &st, Stream &comm);





#endif