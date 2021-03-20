#include <MicroNMEA.h>
#include <math.h>

#define fixCoord(X) ((((double)X) / 1000000.0) * 0.01745329252)

typedef struct pos
{
    long lat;
    long lon;

    void print(Stream &serial) {
      serial.write("Got NMEA DATA ");
      serial.print(lat);
      serial.write(" ");
      serial.print(lon);
      serial.write("\n");
    }

    bool equals(const pos &other) {
        return lat == other.lat && lon == other.lon;
    }

    //http://www.movable-type.co.uk/scripts/latlong.html
    double distance(const pos &other) {
        const long R = 6371e3;  //Earth radius in meters
        double dlat = fixCoord(this->lat) - fixCoord(other.lat);
        double dlon = fixCoord(this->lon) - fixCoord(other.lon);
        double sinlat = sin(dlat/2);
        double sinlon = sin(dlon/2); 
        double a = sinlat*sinlat + cos(fixCoord(this->lat))*cos(fixCoord(other.lat)) * sinlon*sinlon;
        double c = atan2(sqrt(a), sqrt(1 - a));
        return R * c;
    }
} Pos;

Pos readPos(MicroNMEA &nmea) {
    return {.lat = nmea.getLatitude(), .lon = nmea.getLongitude()};
}