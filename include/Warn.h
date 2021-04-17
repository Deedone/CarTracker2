#ifndef WARN_H
#define WARN_H
#include "Storage.h"
#include "Arduino.h"

static const int warn_pins[] = {PD5,PD6,PD7};

class Warn {
    Storage& st;
    Stream& comm;

    void blink_slow(int pin);
    void blink_fast(int pin);
    void turn_on(int pin);
    public:
    Warn(Stream& acomm, Storage& ast) : st(ast), comm(acomm) {
        for(int i = 0; i < TT_NUM; i++){
            pinMode(warn_pins[i],OUTPUT);
        }
    }

    void process_triggers();


};




#endif