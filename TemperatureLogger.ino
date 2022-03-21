#include <OneWire.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <DallasTemperature.h>
#include <SPI.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS2 2
#define ONE_WIRE_BUS4 4
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire2(ONE_WIRE_BUS2);
OneWire oneWire4(ONE_WIRE_BUS4);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors4(&oneWire4);

RTC_DS1307 rtc;
File logfile;
#define sensorPin A0

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  
  // Start up the library
  sensors2.begin();
  sensors4.begin();
  rtc.begin();
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    Serial.print("could not create file");
  }

  logfile.println("datetime,ambient,temp1,temp2,temp3,temp4");    

  
}
 
 
void loop(void)
{
  // Get a reading from the temperature sensor:
  int reading = analogRead(sensorPin);
  // Convert the reading into voltage:
  float voltage = reading * (5000 / 1024.0);
  // Convert the voltage into the temperature in Celsius:
  float ambientTemperature = (voltage - 500) / 10;
  sensors2.requestTemperatures(); // Send the command to get temperatures
  sensors4.requestTemperatures(); // Send the command to get temperatures  
  DateTime time = rtc.now();
 logfile.print(time.timestamp(DateTime::TIMESTAMP_FULL) + "," + ambientTemperature + "," + sensors2.getTempCByIndex(0) + "," + sensors2.getTempCByIndex(1) + "," + sensors2.getTempCByIndex(2) + "," + sensors4.getTempCByIndex(0) + "\r\n");   
 logfile.flush();
 delay(60000);
}
