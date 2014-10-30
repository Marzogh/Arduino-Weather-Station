  /*
    Include information about how the wind sensors operate
    // Activated by the magnet in the anemometer (2 ticks per rotation), attached to input D3
  */
  
  //Read anemometer and get instantaneous wind speed
  float get_wind_speed()
    {
      float dTime = millis()-lastWindChk;    
      dTime /= 1000.0;                                  //Covert ms to sec    
      float windSpeed = (float)windClickNo / dTime;     //3 / 0.750s = 4    
      windClickNo = 0;                                  //Reset and start watching for new wind
      lastWindChk = millis();    
      windSpeed *= 1.492;                               //Calculates the actual wind speed in mph (2 * 1.492 = 2.984mph)    
      return(windSpeed);
    }
    
  //Read wind vane and get current wind direction
  int get_wind_direction()    
    {
      unsigned int adc;    
      adc = avgAnalogRead(wDir); // get the current reading from the sensor
      //The possible wind directions - output in degrees - in relation to the ADC reading are listed below. EAch reading is the midpoint between adjacent readings.
      //Refer to Wind meters datasheet for more information. ( https://www.sparkfun.com/datasheets/Sensors/Weather/Weather%20Sensor%20Assembly..pdf )
      if (adc < 380) return (113);
      if (adc < 393) return (68);
      if (adc < 414) return (90);
      if (adc < 456) return (158);
      if (adc < 508) return (135);
      if (adc < 551) return (203);
      if (adc < 615) return (180);
      if (adc < 680) return (23);
      if (adc < 746) return (45);
      if (adc < 801) return (248);
      if (adc < 833) return (225);
      if (adc < 878) return (338);
      if (adc < 913) return (0);
      if (adc < 940) return (293);
      if (adc < 967) return (315);
      if (adc < 990) return (270);
      return (-1); // error, disconnected?
    }
  
  //Function for wind sensor interrupt 
  void wSpeedIRQ()
  {
    if (!millis()-lastWindIRQ<10)                      //Debounce the wind interrupt switch for 10ms after contact
    {
      lastWindIRQ = millis();                          //Set up last wind interrupt for the next interrupt
      windClickNo++;                                   //Each click per second is 1.492MPH
    }
  }
  
  //Read wind sensors
  void windRead()
  {   
    if(++mins_2 > 119) mins_2 = 0;                        //Take wind speed and direction readings every second and average over 2 minutes

    //Calculate wind speed and direction readings every second and average over 120 seconds
    windspdmph = get_wind_speed();
    winddir = get_wind_direction();
    avgWindSpd[mins_2] = (int)windspdmph;
    avgWindDir[mins_2] = winddir;

    //Check if this is the gust for the minute
    if(windspdmph > WindGust_10min[mins_10])
    {
      WindGust_10min[mins_10] = windspdmph;
      WindGustDir_10min[mins_10] = winddir;
    }

    //Check if this is the gust for the day (this resets every midnight)
    if(windspdmph > windgustmph)
    {
      windgustmph = windspdmph;
      windgustdir = winddir;
    }

    //Blink status LED
    digitalWrite(stat, HIGH);
    delay(25);
    digitalWrite(stat, LOW); 
  }
  
  //Calculate averages of wind speed and direction over 2 mins and averages of wind gusts and direction over 10 mins
  void windCalc()
  {
  //Calculate avg wind speed over 2 min
    float w=0;                                        //Temporary value tolder
    for(int i=0; i<120; i++)
    {
      w+=avgWindSpd[i];
    }
    w/=120;
    windspdmph_avg2m=w;
    
    //Calculate average wind direction over 2 min
    w=0;
    for(int i=0; i<120; i++)
    {
      w+=avgWindDir[i];
    }
    w/=120;
    winddir_avg2m=w;
    
    //Find the largest wind gust in the last 10 minutes and its direction
    windgustmph_10m=0;
    windgustdir_10m=0;
    
    for(int i=0; i<10; i++)
    {
      if(WindGust_10min[i]>windgustmph_10m)
      {
        windgustmph_10m=WindGust_10min[i];
        windgustdir_10m=WindGustDir_10min[i];
      }
    }
  }
