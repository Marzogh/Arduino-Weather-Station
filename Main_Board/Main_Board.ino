

//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````//
//`                                                                                                                                                  `//
//`      \              |        _)                   \ \        /             |    |                       ___|   |           |   _)                `//
//`     _ \     __|  _` |  |   |  |  __ \    _ \       \ \  \   /  _ \   _` |  __|  __ \    _ \   __|     \___ \   __|   _` |  __|  |   _ \   __ \   `//
//`    ___ \   |    (   |  |   |  |  |   |  (   |       \ \  \ /   __/  (   |  |    | | |   __/  |              |  |    (   |  |    |  (   |  |   |  `//
//`  _/    _\ _|   \__,_| \__,_| _| _|  _| \___/         \_/\_/  \___| \__,_| \__| _| |_| \___| _|        _____/  \__| \__,_| \__| _| \___/  _|  _|  `//
//`                                                                                                                                                  `//
//`                An Arduino based Weather station built around the Weather Shield and Weather meters by Sparkfun & the Electric Imp                `//
//`                                                                 by Prajwal Bhattaram                                                             `//
//`                                                                     v.1.1.0                                                                      `//
//`                                                                   30.Oct.2014                                                                    `//
//`                                                                                                                                                  `//
//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````//                                                                                                                                              



//Libraries
#include <Wire.h>                   //I2C for sensors
#include <MPL3115A2.h>              //Pressure sensor
#include <HTU21D.h>                 //Temperature/Humidity sensor
#include <avr/wdt.h>                //Timer watchdog

//Global variables

//Timing variables
  byte seconds;                       //Keeps track of seconds to increment the minute counter
  byte mins_2;                        //Keeps track of time for average wind dir/speed over two minutes
  byte minutes;                       //Keeps track of minutes to enable easy access to time-based array locations
  byte mins_10;                       //Keeps track of time for average wind dir/speed over ten minutes
  long prevSec;                       //Millis counter to check for passing seconds
  unsigned int timeSinceReset;        //Keeps track of time (in mins) since last reset and resets variables after 24hrs

//Weather meter variables
  long lastWindIRQ=0;
  long lastWindChk=0;
  long windClickNo=0;
  
  byte avgWindSpd[120];                //2 minutes of wind speed data - 120 bytes
  byte avgWindDir[120];                //2 minutes of wind direction data - 120 bytes
  volatile float rainHour[60];         //60 minutes of rain data - 60 numbers (float). This is stored as a volatile because the data is retrieved from the RAM.
                                       //Volatile data types can be modified by external services like interrupt routines
  float WindGust_10min[10];            //10 wind gust data values to keep track of the strongest gust in the last 10 minutes
  int WindGustDir_10min[10];           //10 wind direction data values to keep track of thedirection of the strongest gust in the last 10 minutes

//Variables for Wunderground (All the variables Wunderground supports can be found at http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol )
  int winddir;                         //[0-360˚] instantaneous wind direction
  int windspdmph;                      //Instantaneous wind speed, in mph
  int windgustmph;                     //Instantaneous wind gust, in mph
  int windgustdir;                     //[0-360˚] instantaneous wind gust direction
  float windspdmph_avg2m;              //Average wind speed over 2 minutes, in mph
  int winddir_avg2m;                   //[0-360˚] average wind direction over 2 mins
  float windgustmph_10m;               //Average wind gust over 10 mins, im mph
  float windgustdir_10m;               //[0-360˚] average wind gust direction over 10 minutes
  float tempf;                         //Temperature in ˚F
  float humidity;                      //Relative humidity in %
  float pressure;                      //Barometric pressure
  float rainin;                        //Accumulated rain over the past hour, in inches
  volatile float dailyrainin;          //Accumulated rain over the day - so far - in inches
   
  /* 
     It is hard to calculate the atmospheric pressure (in inches/mm of Hg) & dewpoint on the Arduino due to limited processing capabilities and even more limited RAM.
     These calculations will be performed by harnessing the power of the servers the connect to the Electric Imp in the agent code.
     The Electric Imp code for this particular project has been copied as-is from the code written by the wonderful Nathan Seidle of Sparkfun electronics.
     The license information is included in the Electric_Imp_Code_License tab
     The actual code for the Imp is included in this repository in the files named 'agent.nut' and 'device.nut' */
     
//Variables for private use (non-Wunderground)
  float batt_lvl=11.8;                //Set this to the cutoff voltage for your battery setup
  float light_lvl=0.72;               //Cutoff voltage for darkness


  volatile unsigned long lastRain, rainInterval, rain, rainTime;  //These volatiles are subject to modification by RainIRQ
  
  
//Hardware pins

  //Analog pins
  const byte wDir=A0;                  //Wind direction
  const byte light=A1;                 //Light sensor
  const byte batt=A2;                  //Arduino battery pin (Not needed if connected to mains power)
  const byte ref_3V3=A3;               //Arduino 3.3v reference pin
  
  //Digital pins
  const byte RAIN=2;                   //Rain meter
  const byte wSpeed=3;                 //Anemometer
  const byte stat=7;                   //Status LED
  
//Initiate required instances
  MPL3115A2 myPressure;                //Initiate the pressure sensor
  HTU21D myHumidity;                   //Initiate the humidity sensor
  
//Code begins

void setup()
{
  wdt_reset();                         //Reset the timer watchdog - it keeps false alerts from being triggered
  wdt_disable();                       //Disable the timer watchdog - it is not required during the initialization

  Serial.begin(9600);
  //imp.begin(19200);

  pinMode(wSpeed, INPUT_PULLUP);       //Input from anemometer
  pinMode(wDir, INPUT);                //Input from wind vane
  pinMode(RAIN, INPUT_PULLUP);         //Input from rain gauge
  pinMode(light, INPUT);               //Input from light sensor
  pinMode(batt, INPUT);                //Input from battery - to check voltage
  pinMode(ref_3V3, INPUT);              //Input from the 3.3V ref of the Arduino Uno
  pinMode(stat, OUTPUT);               //Status LED is set to Output

  resetDay();                          //Reset rain totals at midnight
  
  configurePressure();                 //Configure pressure sensor
  configureHumidity();                 //Configure humidity sensor
  
  //Attach wind and rain interrupts to the respective IRQ functions
  attachInterrupt(0, rainIRQ, FALLING);
  attachInterrupt(1, wSpeedIRQ, FALLING);
  
  //Initate interrupts
  interrupts();
  
  Serial.print("Arduino Weather Station initiating");
  Serial.print(".");
  Serial.print(".");
  Serial.println(".");
  
  //Report the weather
  weatherReport();
}

void loop()
{
  wdt_reset();                           //Reset the timer watchdog - it keeps false alerts from being triggered
  
  if(millis()-prevSec>=1000)             //Keep a track of time
  {
    prevSec+=1000;
    
    windRead();                          //Read wind sensors
    updateArrays();                      //If 1 minute rolls over then the arrays for windgust and rain are updated
  }
  
  //Wait for Imp to ping
  talkToImp();
  
  //If Imp does not trigger a midnight reset, then force one.
  if(timeSinceReset>(1440+15)) //24h * 60min/h = 1440min + 15 extra mins just in case the Imp triggers the reset 
  {
    resetDay();                          //Reset all the variables that need daily resetting
  }
  delay(100);
}
