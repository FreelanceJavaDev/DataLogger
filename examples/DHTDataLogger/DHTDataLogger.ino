/* IMPORTS */
#include <DHT.h>
#include <DataLoggerSD.h>
/*
 * Full utilization of this library is intended for intermediate users. 
 * This library hides a lot behind the scenes.
 * You must have a shield with an SD card and an RTC connected to your board in addtion to a DHT sensor.
 * The library will do the rest
 */
/* Plese note that the code here assumes use of a DHT11. 
 * If you are using something different select the appropriate DHT type and comment out the others
 * 
 */

/* CONSTANTS and GLOBAL VARIABLES*/
/* Change this to match your SD shield or module:
 * Arduino Ethernet shield: pin 4
 * Adafruit SD shields and modules: pin 10
 * Sparkfun SD shield: pin 8
 */
const uint8_t chipSelect = 4; //change this if needed

/* DHT CONSTANTS*/
#define DHT_TYPE DHT11
//#define DHT_TYPE DHT22
//#define DHT_TYPE DHT21
const uint8_t DHT_PIN = 2;

DataLoggerSD logger(chipSelect, 15, 3); //pin 4, 15 minutes between readings, and pin 3 as an interrupt pin
DHT dht(DHT_PIN, DHT_TYPE); //DHT sensor



void setup() {
  Serial.begin(9600);
  logger.begin();
  //logger.addSensorToList("Temperature", "*C");
  logger.addSensorToList("Temperature", "*F");
  logger.addSensorToList("Humidity", "%");
}

void loop() {
   //float temperature = dht.readTemperature(); //reads temperature in degrees celsius
   float temperature = dht.readTemperature(true); //reads temperature in degrees fahrenheight
   float humidity = dht.readHumidity(); //reads humidity from sensor
   logger.getNewSensorReading("Temperature", temperature); //gets new temperature reading
   logger.getNewSensorReading("Humidity", humidity); //gets new temperature reading
   logger.logData(true); //logs time down to seconds, change to false if you want only minutes.
   logger.collectionInterval(); //ensures data is collected every five minutes.
}



