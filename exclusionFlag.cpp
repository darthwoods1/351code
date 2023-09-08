#include "UARTSerial.h"
#include "cmath"
#include "mbed.h"
#include "TinyGPSplus.h"
#include "initialData.h"

#define GEOFENCE_RADIUS 100.0 // 100 meters
#define EARTH_RADIUS 6371000.0

struct Coordinate {
  float latitude;
  float longitude;
};

Coordinate gateways[] = {
    {34.871278, 150.679803}, {-90.000000, 90.000000}, {32.000000, 52.000000},
    // more coordinates
};

UARTSerial gps_serial(D1, D0);
TinyGPSPlus gps;

float radians(float degree) { return degree * TWO_PI / 180.0; }

float getDistance(float lat1, float lon1, float lat2, float lon2) {
  float dLat = radians(lat2 - lat1);
  float dLon = radians(lon2 - lon1);
  float a = std::sin(dLat / 2) * std::sin(dLat / 2) +
            std::cos(radians(lat1)) * std::cos(radians(lat2)) *
                std::sin(dLon / 2) * std::sin(dLon / 2);
  float c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
  return c * EARTH_RADIUS;
}

bool exclusionFlag() {

  char gps_buffer[128];

  while (true) {
    int len = gps_serial.read(gps_buffer, sizeof(gps_buffer) - 1);
    gps_buffer[len] = '\0';
    for (int i = 0; i < len; i++) {
      gps.encode(gps_buffer[i]);
    }

    if (gps.location.isValid()) {
      float latitude = gps.location.lat();
      float longitude = gps.location.lng();
      for (int i = 0; i < sizeof(gateways) / sizeof(gateways[0]); ++i) {
        float distance = getDistance(
            gateways[i].latitude, gateways[i].longitude, latitude, longitude);
        if (distance <= GEOFENCE_RADIUS) {
          return 1;
          break;
        } else {
          return 0;
        }
      }
    }
  }
}