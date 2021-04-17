#include <Arduino.h>
#include <MicroNMEA.h>
#include <SoftwareSerial.h>
#include <Ring.h>
#include <GNSS.h>
#include <Storage.h>
#include <Control.h>
#include <Warn.h>



SoftwareSerial gnss(2,3);
Storage st;
Warn warn(Serial, st);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  gnss.begin(9600);
  Serial.write("Boot OK\n");
  st.init();
  double distance = st.get_distance();
  Serial.print(F("Saved distance: "));
  Serial.print(distance);
  Serial.print(F("\n"));
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  double diff = processNMEA(gnss);
  st.set_distance(st.get_distance() + diff);
  processCommands(st, Serial);
  warn.process_triggers();

  delay(10);
}