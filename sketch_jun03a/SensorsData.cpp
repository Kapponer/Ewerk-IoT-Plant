// SensorsData.cpp
#include "SensorsData.h"
#include <Arduino_MKRIoTCarrier.h>

//Implementation from the class
String SensorsData::ToCsvString()
{
  float temperature = SensorsData::Temperature;
  float humidity = SensorsData::Humidity;
  int lightIntensity = SensorsData::LightIntensity;
  return String(String(temperature) + "," + String(humidity) + "," + String(lightIntensity));
}
