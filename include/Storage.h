#ifndef STORAGE_H
#define STORAGE_H
#include <EEPROM.h>

#define STORAGE_MAGIC 0xdeadf00dUL
#define NUM_TRIGGERS 32

enum trigtype {
    TT_WARN,
    TT_ERR,
    TT_CRIT,
    TT_NUM
};

struct trigger {
    char type;
    double distance;
};

class Storage {
    unsigned long magic;
    unsigned long writes;
    unsigned long valid_triggers;
    double distance;
    trigger triggers[NUM_TRIGGERS];

    void maybe_relocate();
    void maybe_save();
    void save();
public:
    void init(bool reinit = false);
    void set_distance(double dist);
    double get_distance();


    bool add_trigger(char type, double distance);
    bool is_trigger_valid(int index);
    trigger get_trigger(int index);
    void del_trigger(int index);
    void set_trigger(int index, char type, double distance);
};


#endif