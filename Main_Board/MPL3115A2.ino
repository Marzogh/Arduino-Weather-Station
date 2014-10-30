 //Configure sensor
 void configurePressure()
 {
  myPressure.begin();                     // Activate sensor
  myPressure.setModeBarometer();          // Set sensor to measure pressure in Pascals (20 to 110 kPa)
  myPressure.setOversampleRate(128);      // Set the oversample rate to 128 (recommended)
  myPressure.enableEventFlags();          // Enable all three temperature and pressure event flags 
  myPressure.setModeActive();             // Go active and start taking measurements
 }
 
 //Calculate temperature and pressure
 void temp_pressureCalc()
 {
   tempf=myPressure.readTempF();
   pressure = myPressure.readPressure();
 }
