// SensorsData.h

#ifndef SensorsData_h
#define SensorsData_h

#include <Arduino.h>

// Class for all important sensors
class SensorsData {
  private:

  public:
    float Temperature;
    float Humidity;
    int LightIntensity;
    String ToCsvString();
};

#endif
