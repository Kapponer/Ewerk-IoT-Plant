#include <Arduino_MKRIoTCarrier.h>
#include "SensorsData.h"

MKRIoTCarrier carrier;
File csvSensorData;

// name of the file doesn't be longer then 12 chars.
String filename = "data.csv";
bool LoggingIsOn = true;
int touchNumber;
unsigned long timeButtonPressed;
unsigned long timeLastMeasurement;
SensorsData sensorDataSum;
int numberOfMeasurements;

// is just called once at the start
void setup()
{
  // if the carrier is in a case or not
  CARRIER_CASE = true;
  // must be called to get all sensors and buttons on the carrier
  carrier.begin();
  SetGlobalCarrierSettings();
  {
    SD.remove(filename);
  }

  CheckIfSdCardIsConnected();
  csvSensorData = SD.open(filename, FILE_WRITE);
  csvSensorData.println("millis, temperature, humidity, lightIntensity");
}

// the loop function runs over and over again forever
void loop()
{
  // updates buttons status
  carrier.Buttons.update();
  
  if (LogIfFileExists())
  {
    if (carrier.Buttons.onTouchDown(TOUCH0))
    {
      Serial.println("Button 0 pressed down");
      touchNumber = 0;
      timeButtonPressed = millis();
      ToggleLogging(0);
    }

    if (carrier.Buttons.onTouchDown(TOUCH2))
    {
      Serial.println("Button 2 pressed down");
      touchNumber = 2;
      timeButtonPressed = millis();
      PlantAnalyzer(2);
    }

    ResetTouchNumberAfter(5000);

    // toggle write data
    if (LoggingIsOn)
    {
        
      // 300000 = 5min
      // 120000 = 2min
      WriteSensorData(120000);
    }
  }
}

void ToggleLogging(int inputNumber)
{
  String loggingText = "enable";
  if (LoggingIsOn)
  {
    loggingText = "disable";
  }
  
  if (touchNumber == inputNumber)
  {
    if (LoggingIsOn)
    {
      LoggingIsOn = false;
    }
    else
    {
      LoggingIsOn = true;
    }
    String DisplayText = String("Logging is " + loggingText + "!");
    PrintOnDisplay(DisplayText);
  }
  else
  {
    String DisplayText = String("Press again to " + loggingText + " logging!");
    PrintOnDisplay(DisplayText);
  }
}

void PlantAnalyzer(int inputNumber)
{
  // Durchschnitt aus den Kategorien bilden
  float averageTemperature = sensorDataSum.Temperature / numberOfMeasurements;
  float averageHumidity = sensorDataSum.Humidity / numberOfMeasurements;
  float averageLightIntensity = sensorDataSum.LightIntensity / numberOfMeasurements;
  // Anhand von Grenzen festlegen, welche Pflanze gewählt werden sollte
  String text = String("Temp~: " + String(averageTemperature) + "°C; Hum~: " + String(averageHumidity) + "%; Light~: " + String(averageLightIntensity));
  PrintOnDisplay(text);
  Serial.println(text);
}

void ResetTouchNumberAfter(unsigned long resetTime)
{
  unsigned long currentTime = millis();
  if (currentTime >= timeButtonPressed + resetTime)
  {
    touchNumber = -1;
  }
}

void WriteSensorData(unsigned long delayTime)
{
  unsigned long currentTime = millis();
  if ( currentTime >= timeLastMeasurement + delayTime)
  {
    timeLastMeasurement = millis();
    SensorsData sensorsData;
    sensorsData.Temperature = GetCurrentTemperature();
    sensorsData.Humidity = GetCurrentHumidity();
    sensorsData.LightIntensity = GetCurrentLightIntensity();
  
    sensorDataSum.Temperature = sensorDataSum.Temperature + sensorsData.Temperature;
    sensorDataSum.Humidity = sensorDataSum.Humidity + sensorsData.Humidity;
    sensorDataSum.LightIntensity = sensorDataSum.LightIntensity + sensorsData.LightIntensity;
    numberOfMeasurements++;
    Serial.println(numberOfMeasurements);
  
    String sensorData = String(String(millis()) + ", " + sensorsData.ToCsvString());
    SaveDataOnSdCard(sensorData);
  
    PrintOnDisplay(sensorData);
    Serial.println(sensorData);
  }
}

void SetGlobalCarrierSettings()
{
  carrier.display.setTextSize(1);
  carrier.display.setCursor(0, 120);
  carrier.leds.setBrightness(64);
}

void SaveDataOnSdCard(String data)
{
  csvSensorData.println(data);
  csvSensorData.flush();
}

bool LogIfFileExists()
{
  if (SD.exists(filename))
  {
    //Serial.println("data.csv exists.");
    return true;
  } else
  {
    //Serial.println("data.csv doesn't exist.");
    return false;
  }
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

unsigned int GetCurrentLightIntensity()
{
  if (carrier.Light.colorAvailable())
  {
    int r, g, b, lightIntensity;
    carrier.Light.readColor(r, g, b, lightIntensity);
    return lightIntensity;
  }

  // hier besteht Loopgefahr
  return GetCurrentLightIntensity();
}
