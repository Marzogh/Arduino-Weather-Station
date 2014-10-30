  //Calculate battery voltage - if station is battery powered
  void battCalc()
  {
    float opVolt=avgAnalogRead(ref_3V3);
    float rawVolt=avgAnalogRead(batt);
    opVolt=3.30/opVolt;                           //Ref voltage is 3.3V
    rawVolt*=opVolt;                              //Convert the integer to actual voltage on the batt pin
    rawVolt*=4.90;                                //Multiply the batt voltage by the voltage divider resistance [(3.9k+1k)/1K] to get the real system voltage
    batt_lvl=rawVolt;
  }
  
  //Average a number of Analog readings
  
  int avgAnalogRead(int pin)
    {
      byte noOfReadings=10;
      unsigned int tempValue=0;
      for(int x=0 ; x<noOfReadings ; x++)
        tempValue+=analogRead(pin);
      tempValue/=noOfReadings;
    
      return(tempValue);  
    }
    
    
