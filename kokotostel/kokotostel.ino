#include <dht.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"
#include <SD.h>

const int chipSelect = 4;
dht DHT;
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define DHT11_PIN 2
#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


void setup(){
  Serial.begin(57600);
  sensors.begin();
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
   Serial.println("Card failed, or not present");
   // don't do anything more:
   return;
 }
 Serial.println("card initialized.");
   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop()
{ 
  sensors.requestTemperatures(); 
  Serial.print("Aisththras Edafous: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("\n"); 
  delay(1000);

  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Genikh Thermokrasia = ");
  Serial.println(DHT.temperature);
  Serial.print("Ygrasia = ");
  Serial.println(DHT.humidity);
  
  DateTime now = rtc.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
 if (dataFile) {
   dataFile.print(DHT.temperature);
   dataFile.print(',');
   dataFile.print(DHT.humidity);
   dataFile.print(',');
   dataFile.print(sensors.getTempCByIndex(0));
   dataFile.print(',');
   dataFile.print(now.day(), DEC);
   dataFile.print('/');
   dataFile.print(now.month(), DEC);
   dataFile.print('/');
   dataFile.print(now.year(), DEC);
   dataFile.print(',');
   dataFile.print(now.hour(), DEC);
   dataFile.print(':');
   dataFile.print(now.minute(), DEC);
   dataFile.print(':');
   dataFile.print(now.second(), DEC);   
   dataFile.println("");
   dataFile.close();
   
 }  
 else {
   Serial.println("error opening datalog.txt");
 } 
  delay(1800000);
}
