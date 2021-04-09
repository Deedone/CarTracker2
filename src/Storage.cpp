#include <Storage.h>
#include <Arduino.h>

struct privdata {
    int saveaddr;
    unsigned long lastsave;
};

privdata priv;
#define SAVE_DELAY ((1000UL * 60UL) )

//Relocate struct every 100k writes to reduce eeprom wear
void Storage::maybe_relocate() {
    if(writes > 100000) {
        priv.saveaddr += 41;
        if (priv.saveaddr + sizeof(Storage) >= 1024) {
            priv.saveaddr =  (priv.saveaddr + sizeof(Storage)) % 1024;
        }
        Serial.print("Relocating\n");
        save();
    }
}
void Storage::maybe_save() {
    maybe_relocate();
    unsigned long now = millis();
    //Serial.print("Check save ");
    //Serial.print(now);
    //Serial.print(" ");
    //Serial.print(priv.lastsave);
    //Serial.print(" ");
    //Serial.print(abs(now - priv.lastsave));
    //Serial.print(" ");
    //Serial.print(SAVE_DELAY);
    //Serial.print("\n");
    if (abs(now - priv.lastsave) > SAVE_DELAY) {
        save();
    }
}
void Storage::save() {
    Serial.print(F("Saving\n"));
    bool actual_write = false;
    char *ptr = (char*)this;
    int size = sizeof(Storage);
    for(int i = 0; i < size; i++) {
        if(EEPROM[i] != ptr[i]) {
            actual_write =true;
            EEPROM[i] = ptr[i];
        }
    }
    priv.lastsave = millis();
    if(actual_write) {
        writes++;
    }
}

void Storage::init(bool reinit) {

    for(int i = 0; i < 1024 - sizeof(Storage); i++) {
        unsigned int test = 0;
        EEPROM.get(i, test);
        if (test == STORAGE_MAGIC) {
            priv.saveaddr = i;
        }
        Storage tmp;
        Serial.print(F("Loading\n"));
        EEPROM.get(i, tmp);
        memcpy(this, &tmp, sizeof(Storage));
        return;
    }
    memset(this, 0, sizeof(Storage));
    magic = STORAGE_MAGIC;
    distance = 0.0;
    save();
}
void Storage::set_distance(double dist) {
    distance = dist;
    maybe_save();
}
double Storage::get_distance() {
    return distance;
}

bool Storage::add_trigger(char type, double distance) {
    int i;
    for(i = 0; i < NUM_TRIGGERS; i++) {
        if (!(valid_triggers & (1 << i))) {
            break;
        }
    }
    if (i == NUM_TRIGGERS) {
        return false;
    }

    triggers[i].type = type;
    triggers[i].distance = distance;
    valid_triggers |= (1 << i);
    maybe_save();
    return true;
}

trigger Storage::get_trigger(int index) {
    return triggers[index];
}
void Storage::set_trigger(int index, char type, double distance) {
    if (index >= NUM_TRIGGERS || index < 0) {
        return;
    }
    triggers[index].type = type;
    triggers[index].distance = distance;
    valid_triggers |= (1 << index);
    maybe_save();
}
void Storage::del_trigger(int index) {
    triggers[index].type = 0;
    triggers[index].distance = 0;
    valid_triggers &= ~(1 << index);
    maybe_save();
}

bool Storage::is_trigger_valid(int index) {
    return valid_triggers & (1 << index);
}