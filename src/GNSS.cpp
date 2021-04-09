#include <GNSS.h>
#include <Ring.h>

char nmeabuf[100];
MicroNMEA nmea(nmeabuf, sizeof(nmeabuf));
Ring<Pos, 50> points;

double distance = 0;
const int batch = 30;
const float threshold = 0.15; // 20%
int unprocessed = 0;

static Pos readPos(MicroNMEA &nmea) {
    return {.lat = nmea.getLatitude(), .lon = nmea.getLongitude()};
}

static void collectPoint(Pos p) {
      //Skip identical readings
      if (p.equals(points.getLast(0)))
        return;
      p.print(Serial);
      points.put(p);
      unprocessed++;
}

static bool pointDataReady() {
  return unprocessed >= batch;
}

double processNMEA(Stream &gnss) {
  while (gnss.available()) {
    char c = gnss.read();
    if (nmea.process(c)) {
      Pos p = readPos(nmea);
      collectPoint(p);
      if (!pointDataReady())
        continue;
      double fine = 0;
      double crude = points.getLast(0).distance(points.getLast(unprocessed-1));
      for(int i = 0; i < unprocessed - 1; i++) {
        fine += points.getLast(i).distance(points.getLast(i + 1));
      }
      unprocessed = 0;
      Serial.print(F("fine:"));
      Serial.print(fine);
      Serial.print(F(" crude:"));
      Serial.print(crude);
      Serial.print(F(" diff:"));
      Serial.print(abs((fine/crude) - 1.0));
      Serial.print(F("\n"));
      //In perfect case the ratio is one os diff is 0
      if (abs((fine/crude) - 1.0) > threshold || fine > 1000 || crude > 1000) {
        Serial.print(F("Difference too big\n"));
        continue;
      }
      return fine;
      //distance += fine;
      //Serial.print(F("Total distance "));
      //Serial.print(distance);
      //Serial.print(F("\n"));
    }
  }
  return 0;
}