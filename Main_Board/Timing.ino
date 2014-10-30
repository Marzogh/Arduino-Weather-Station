  //Wind gust and rainfall arrays are updated every minute
  void updateArrays()
  {
    if(++seconds > 59)
    {
      seconds = 0;
      if(++minutes > 59) minutes = 0;
      if(++mins_10 > 9) mins_10 = 0;
      WindGust_10min[mins_10] = 0;                        //Zero out wind gust for this minute
      rainHour[minutes] = 0;                         //Zero out rainfall for this minute
      timeSinceReset++;                              //Add one minute to the time since the last midnight reset   
    }
  }
  
  //Reset all dailiy variables at midnight
  void resetDay()
  {
    dailyrainin = 0;                                 //Reset daily rain total
    windgustmph = 0;                                    //Reset the daily windgust
    windgustdir = 0;                                 //Reset the daily gust direction
    minutes = 0;                                     //Reset the minute tracker
    seconds = 0;                                     //Reset the second tracker
    prevSec = millis();                              //Reset the variable used to track minutes
    
    timeSinceReset = 0;                              //Reset the emergency midnight reset
  }
