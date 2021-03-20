#include <Arduino.h>
#include <MicroNMEA.h>
#include <SoftwareSerial.h>
#include <Ring.h>
#include <GNSS.h>


SoftwareSerial gnss(2,3);
char nmeabuf[100];
MicroNMEA nmea(nmeabuf, sizeof(nmeabuf));
Ring<Pos, 50> points;
double distance = 0;

void processNMEA() {
  while (gnss.available()) {
    char c = gnss.read();
    Serial.write(c);
    if (nmea.process(c)) {
      Pos p = readPos(nmea);
      //Skip identical readings
      if (p.equals(points.getLast(0)))
        continue;
      p.print(Serial);
      points.put(p);
      double d = p.distance(points.getLast(1));
      //Skip sudden changes of over 100 meters
      if (d > 100)
        continue;
      //Filter noise
      if (d < 0.5)
        continue;
      distance += d;
      Serial.write("Total distance ");
      Serial.print(distance);
      Serial.write("\n");
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  gnss.begin(9600);
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  processNMEA();
  while (Serial.available()) {
    distance = 0;
    Serial.read();
  }
  delay(10);
}