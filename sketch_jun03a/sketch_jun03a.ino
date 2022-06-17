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
  
  CheckIfSdCardIsConnected();
  csvSensorData = SD.open("sensorsData.csv", FILE_WRITE);
  csvSensorData.println("millis, temperature, humidity, lightIntensity");
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
  csvSensorData.close();
  PrintOnDisplay(logData);
  delay(10000);
}

void CheckIfSdCardIsConnected()
{
  const int chipSelect = SD_CS;
  Sd2Card card;
  Serial.print("\nInitializing SD card...");
  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }
}

void PrintOnDisplay(String text)
{
  ResetDisplay();
  carrier.display.print(text);
}

void ResetDisplay()
{
  carrier.display.fillScreen(000000);
  carrier.display.setCursor(0, 120);
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
  if (carrier.Light.colorAvailable())
  {
    int r, g, b, lightIntensity;
    carrier.Light.readColor(r, g, b, lightIntensity);
    return lightIntensity;
  }
  return 0;
}
