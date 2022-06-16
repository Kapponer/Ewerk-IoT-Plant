#include <Arduino_MKRIoTCarrier.h>
#include "SensorsData.h"

MKRIoTCarrier carrier;
File csvSensorData;

// is just called once at the start
void setup()
{
  // if the carrier is in a case or not
  CARRIER_CASE = true;
  // must be called to get all sensors and buttons on the carrier
  carrier.begin();
  carrier.display.setTextSize(1);
  carrier.display.setCursor(0, 120);
  carrier.leds.setBrightness(64);

  csvSensorData = SD.open("sensorsData.csv", FILE_WRITE);
  csvSensorData.println("Millis, temperature, humidity, lightIntensity");
}

// the loop function runs over and over again forever
void loop()
{
  SensorsData sensorsData;
  sensorsData.Temperature = GetCurrentTemperature();
  sensorsData.Humidity = GetCurrentHumidity();
  sensorsData.LightIntensity = GetCurrentLightIntensity();

  String milliseconds = String(millis());
  String logData = String(milliseconds + ", " + sensorsData.ToCsvString());
  csvSensorData.println(logData);
  carrier.display.print(logData);
  delay(10000);
}

void ShowGreenLight()
{
  uint32_t green = carrier.leds.Color(255, 0, 0);
  carrier.leds.fill(green, 0, 5);
  carrier.leds.show();
}

void ShowRedLight()
{
  uint32_t red = carrier.leds.Color(0, 255, 0);
  carrier.leds.fill(red, 0, 5);
  carrier.leds.show();
}

float GetCurrentTemperature()
{
  return carrier.Env.readTemperature();
}

float GetCurrentHumidity()
{
  return carrier.Env.readHumidity();
}

float GetCurrentPressure()
{
  return carrier.Pressure.readPressure();
}

int GetCurrentLightIntensity()
{
  if(carrier.Light.colorAvailable())
  {
    int r,g,b,lightIntensity;
    carrier.Light.readColor(r,g,b,lightIntensity);
    return lightIntensity;
  }
  return 0;
}
