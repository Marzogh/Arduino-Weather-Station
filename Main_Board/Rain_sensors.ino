  /*
    Include information about how the rain gauge operates
    // Count rain gauge bucket tips as they occur
    // Activated by the magnet and reed switch in the rain gauge, attached to input D2
  */
  
  void rainIRQ()
  {
    //rainTime=millis();                        //Get the current time
    //rainInterval=rainTime-lastRain;           //Calculate the time between this rain interrupt and the last
    
      if (!millis()-lastRain<10)                //Debounce the rain interrupt switch for 10ms after contact
      {
        lastRain=millis();                       //Set up last rain interrupt for the next interrupt
        dailyrainin+=0.011;                     //Each bucket tip is 0.011" of water
        rainHour[minutes]+=0.11;                //Increase the amount of rain for the current minute
      }
  }
  
  //Calculate the rain in the last hour - total rainfall for the day is calculated in the IRQ above
  void rainCalc()
  {
    rainin=0;
    for (int i=0; i<60; i++)
    {
      rainin+=rainHour[i];
    }
  }
