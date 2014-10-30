  //Configure the sensor
  void configureHumidity()
  {
    myHumidity.begin();                      //Initiates the humidity sensor
  }
  
  //Calculate humidity
  
  void humidityCalc()
  {
    humidity=myHumidity.readHumidity();
  }
