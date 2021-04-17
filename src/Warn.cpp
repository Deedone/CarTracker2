#include "Warn.h"

void Warn::blink_slow(int pin) {
    static unsigned long last_millis = 0;
    unsigned long mil = millis();
    if (mil - last_millis > 1000) {
        last_millis = millis();
        digitalWrite(pin, !digitalRead(pin));
    }
}

void Warn::blink_fast(int pin){
    static unsigned long last_millis = 0;
    unsigned long mil = millis();
    if (mil - last_millis > 500) {
        last_millis = millis();
        digitalWrite(pin, !digitalRead(pin));
    }
}

void Warn::turn_on(int pin){
    digitalWrite(pin, 1);
    
}

void Warn::process_triggers() {
    int maxtype = -1;
    for (int i = 0; i < NUM_TRIGGERS; i++){
        if(st.is_trigger_valid(i) && st.get_distance() > st.get_trigger(i).distance) {
            maxtype = max(maxtype, st.get_trigger(i).type);
        }
    }
    switch (maxtype) {
        case TT_WARN:
            blink_slow(warn_pins[TT_WARN]);
            break;
        case TT_ERR:
            blink_fast(warn_pins[TT_ERR]);
            break;
        case TT_CRIT:
            turn_on(warn_pins[TT_CRIT]);
            break;
    default:
        break;
    }


}