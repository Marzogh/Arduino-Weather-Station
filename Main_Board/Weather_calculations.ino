  //Calculates all the variables required by Wunderground
  
  void calcWeather()
  {
    windCalc();
    rainCalc();
    humidityCalc();
    temp_pressureCalc();
    lightCalc();
    battCalc();
  }
  
  void weatherReport()
  {
    calcWeather();
    //Output all variables needed by Wunderground to Serial so they are transmitted by the Electric Imp
    Serial.print("$,winddir=");
    Serial.print(winddir);
    Serial.print(",windspdmph=");
    Serial.print(windspdmph, 1);
    Serial.print(",windgustmph=");
    Serial.print(windgustmph, 1);
    Serial.print(",windgustdir=");
    Serial.print(windgustdir);
    Serial.print(",windspdmph_avg2m=");
    Serial.print(windspdmph_avg2m, 1);
    Serial.print(",windDir_avg2m=");
    Serial.print(winddir_avg2m);
    Serial.print(",windgustmph_10m=");
    Serial.print(windgustmph_10m, 1);
    Serial.print(",windgustdir_10m=");
    Serial.print(windgustdir_10m);
    Serial.print(",humidity=");
    Serial.print(humidity, 1);
    Serial.print(",tempf=");
    Serial.print(tempf, 1);
    Serial.print(",rainin=");
    Serial.print(rainin, 2);
    Serial.print(",dailyrainin=");
    Serial.print(dailyrainin, 2);
    Serial.print(","); //Don't print pressure= because the agent will be doing calcs on the number
    Serial.print(pressure, 2);
    Serial.print(",batt_lvl=");
    Serial.print(batt_lvl, 2);
    Serial.print(",light_lvl=");
    Serial.print(light_lvl, 2);
    Serial.print(",");
    Serial.println("#,");
  }
