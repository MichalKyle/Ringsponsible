#include <SimbleeBLE.h>
 
// pin 3 on the RGB shield is the green led
int led = 3;
 
// interval between advertisement transmissions ms (range is 20ms to 10.24s) - default 20ms
int interval = 675;  // 675 ms between advertisement transmissions
 
// time to advertise in milliseconds
int duration = SECONDS(5);
 
void advertise(const char *data, uint32_t ms)
{
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  SimbleeBLE.advertisementData = data;
  
  // start the BLE stack
  SimbleeBLE.begin();
  
  // advertise for ms milliseconds
  Simblee_ULPDelay(ms);
  
  // stop the BLE stack
  SimbleeBLE.end();
}
 
void setup() {
  // led used to indicate that the Simblee is advertising
  pinMode(led, OUTPUT);
 
  // change the advertisement interval
  SimbleeBLE.advertisementInterval = interval;
 
  SimbleeBLE.advertisementData = "000";
 
  // start the BLE stack
  SimbleeBLE.begin();
