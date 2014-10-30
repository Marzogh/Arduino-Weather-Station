  //Calculate the light level
  void lightCalc()
  {
    float opVolt=avgAnalogRead(ref_3V3);
    float lightSensor=avgAnalogRead(light);
    opVolt=3.3/opVolt;
    lightSensor*=opVolt;
    light_lvl=lightSensor;
  }
