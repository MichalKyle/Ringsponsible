// We do not even need a signal to indicate a phone call, starting connection will do the job

#include <SimbleeBLE.h>
 
int connection_led = 2;
 
void setup() {
  // led used to indicate that the Simblee is connected
  pinMode(connection_led, OUTPUT);
 
  // start the BLE stack
  SimbleeBLE.begin();
  
}
 
void loop() {
  // switch to lower power mode
  Simblee_ULPDelay(INFINITE);
}
 
void SimbleeBLE_onConnect()
{
  digitalWrite(connection_led, HIGH);
}
 
void SimbleeBLE_onDisconnect()
{
  digitalWrite(connection_led, LOW);
}
