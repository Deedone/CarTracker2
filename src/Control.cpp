#include "Control.h"
#include "Arduino.h"
#include "Storage.h"
#include "GNSS.h"

unsigned char cmdbuf[128];
int cmdi = 0;

static void do_reset(Storage &st, Stream &comm) {
         comm.write("Resetting the counter\n");
         distance = 0;
         st.set_distance(distance);
}

static void do_getdist(Storage &st, Stream &comm) {
    Serial.println(st.get_distance());
}

static void do_gettrigs(Storage &st, Stream &comm) {
    for (int i = 0; i < NUM_TRIGGERS; i++) {
        if (st.is_trigger_valid(i)) {
            trigger t = st.get_trigger(i);
            comm.print(i);
            comm.print(F(" "));
            comm.print((int)t.type);
            comm.print(F(" "));
            comm.print(t.distance);
            comm.print(F("\n"));
        }
    }
    comm.print(F("END OK\n"));
}

static void do_addtrig(Storage &st, Stream &comm) {
    char *ptr = strtok((char*)cmdbuf, " ");
    ptr = strtok(NULL, " ");
    if (!ptr) {
        comm.print(F("END ERR\n"));
        return;
    }
    int type = String(ptr).toInt();
    ptr = strtok(NULL, " ");
    if (!ptr) {
        comm.print(F("END ERR\n"));
        return;
    }
    double distance = String(ptr).toDouble();

    if (type >= TT_NUM) {
        comm.print(F("END ERR\n"));
        return;
    }

    for(int i = 0; i < NUM_TRIGGERS; i++) {
        if (!st.is_trigger_valid(i)) {
            st.set_trigger(i, type, distance);
            comm.print(F("END OK\n"));
            return;

        }
    }
    comm.print(F("END ERR\n"));
}

static void do_deltrig(Storage &st, Stream &comm) {
    char *ptr = strtok((char*)cmdbuf, " ");
    ptr = strtok(NULL, " ");
    if (!ptr) {
        comm.print(F("END ERR\n"));
        return;
    }
    int ind = String(ptr).toInt();
    st.del_trigger(ind);
    comm.print(F("END OK\n"));
}

static void do_setdist(Storage &st, Stream &comm) {
    comm.print("Do setdist");
    double newdist = String((char*)cmdbuf+1).toDouble();
    comm.print("Do newdist" + String(newdist));
    st.set_distance(newdist);

    comm.print(F("END OK\n"));
}

static void parse_cmd(Storage &st, Stream &comm) {
    switch (cmdbuf[0]) {
        case CMD_RESET:
            do_reset(st, comm);
            break;
        case CMD_GETDIST:
            do_getdist(st,comm);
            break;
        case CMD_SETDIST:
            do_setdist(st, comm);
            break;
        case CMD_GETTRIGS:
            do_gettrigs(st, comm);
            break;
        case CMD_ADDTRIG:
            do_addtrig(st, comm);
            break;
        case CMD_DELTRIG:
            do_deltrig(st, comm);
            break;
        default:
            comm.print(F("UNKNOWN COMMAND "));
            comm.println(cmdbuf[0]);

    }
}

void processCommands(Storage &st, Stream &comm) {
  while(comm.available()) {
    unsigned char inp = Serial.read();
    if (inp == '\r') {
        continue;
    }else if (inp == CMD_END || cmdi == sizeof(cmdbuf) - 1) {
        parse_cmd(st, comm);
        memset(cmdbuf, 0, sizeof(cmdbuf));
        cmdi = 0;
    } else {
        cmdbuf[cmdi++] = inp;
    }
  }
}
