  //Code for resolving a ping from the Imp
  void talkToImp()
  {
    if(Serial.available())
  {
    byte incoming = Serial.read();
    if(incoming == '!')                        //Indicates the Imp is ready to accept new readings
    {
      weatherReport();                         //Send current readings to Imp for posting to Wunderground (196ms).
    }
    else if(incoming == '@')                   //Indicates midnight local time
    {
      resetDay();                              //Reset all the variables that need daily resetting
    }
    else if(incoming == '#')                   //Indicates a hardware reset
    {
      delay(5000);                             //Trigger a hardware reset via the watchdog timer 
    }
  }
  }
