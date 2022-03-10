#include <OneWire.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <DallasTemperature.h>
#include <SPI.h>
 
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

RTC_DS1307 rtc;
File logfile;
#define sensorPin A0

void setup(void)
{
  // start serial port
  Serial.begin(9600);
  
  // Start up the library
  sensors.begin();
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

  logfile.println("datetime,ambient,temp1,temp2,temp3");    

  
}
 
 
void loop(void)
{
  // Get a reading from the temperature sensor:
  int reading = analogRead(sensorPin);
  // Convert the reading into voltage:
  float voltage = reading * (5000 / 1024.0);
  // Convert the voltage into the temperature in Celsius:
  float ambientTemperature = (voltage - 500) / 10;
  sensors.requestTemperatures(); // Send the command to get temperatures  
  DateTime time = rtc.now();
 logfile.print(time.timestamp(DateTime::TIMESTAMP_FULL) + "," + ambientTemperature + "," + sensors.getTempCByIndex(0) + "," + sensors.getTempCByIndex(1) + "," + sensors.getTempCByIndex(2) + "," + sensors.getTempCByIndex(3) + "\r\n");   
 logfile.flush();
 delay(60000);
}
