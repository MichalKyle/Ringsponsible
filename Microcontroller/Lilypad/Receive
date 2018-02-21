// This code is the simply vertion of presenting heart beat signal from iphone to the flash LED

#include <SimbleeBLE.h>

int flshing_led = 2;

void setup() {
  // led used to indicate that the Simblee is connected
  pinMode(flshing_led, OUTPUT);
  
  SimbleeBLE.txPowerLevel = -4;  // (-20dbM to +4 dBm)

  // start the BLE stack
  SimbleeBLE.begin();
  
}

void SimbleeBLE_onReceive(char *data, int len)
{
}

void loop() {
  Simblee_ULPDelay(200); //delay 200 ms between each signal
  
  if (data[0] && data[1])
  {
    for (int i = 3; i < 37; i++)
    {
      if (data[i])
        digitalWrite(flshing_led, HIGH);
      else
        digitalWrite(flshing_led, LOW);
    }
  }
}
