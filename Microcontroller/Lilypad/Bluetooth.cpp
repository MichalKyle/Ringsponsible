public class Bluetooth
{
  // Data
  public int i = 0;
  // Function
  public bool advertise(const char *data, uint32_t ms)
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

    return true;
  }
};

