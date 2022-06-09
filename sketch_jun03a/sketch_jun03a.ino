#include <Arduino_MKRIoTCarrier.h>
MKRIoTCarrier carrier;

void setup()
{
  CARRIER_CASE = true;
  carrier.begin();
  carrier.display.setTextSize(2);
  carrier.display.setCursor(0, 120);
  carrier.leds.setBrightness(64);
}

// the loop function runs over and over again forever
void loop()
{
  String displayText;
  bool buttonPressed = false;
  // updates buttons status
  carrier.Buttons.update();
  if (carrier.Buttons.onTouchDown(TOUCH0))
  {
    buttonPressed = true;
    ShowGreenLight();
    displayText = "Gruenes Licht";
  }
  else if (carrier.Buttons.onTouchDown(TOUCH1))
  {
    buttonPressed = true;
    ShowRedLight();
    displayText = "Rotes Licht";
  }
  else if (carrier.Buttons.onTouchDown(TOUCH2))
  {
    buttonPressed = true;
    displayText = String("Luftfeuchtigkeit: " + String(GetCurrentHumidity()));
  }
  else if (carrier.Buttons.onTouchDown(TOUCH3))
  {
    buttonPressed = true;
    displayText = String("Druck: " + String(GetCurrentPressure()));
  }
  else if (carrier.Buttons.onTouchDown(TOUCH4))
  {
    buttonPressed = true;
    displayText = String("Temp.: " + String(GetCurrentTemperature()));
  }

  if (buttonPressed)
  {
    ButtonPressedDefault();
    //reset text on display
    carrier.display.fillScreen(000000);
    carrier.display.setCursor(0, 120);
    carrier.display.print(displayText);
  }
}

void ButtonPressedDefault()
{
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
