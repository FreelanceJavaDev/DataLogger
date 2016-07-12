/* IMPORTS */
#include <DataLoggerSD.h>
/* Plese note that the code here assumes use of a 6 volt solar panel. 
 * If you are using something different adjust VoltageDividerR1 and voltageDividerR2 
 * so that between the second and ground there is a MAXIMUM of 5 volts
 * 
  */

/* CONSTANTS and GLOBAL VARIABLES*/
// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const uint8_t chipSelect = 4; //change this
DataLoggerSD logger(chipSelect);

//Solar Panel constants
const float voltageDividerR1 = 2950.0; //3k ohm resistor 

const float voltageDividerR2 = 14700; //15k ohm resistor
const uint8_t analogPin = 0; //

float solarPanelVoltage; //solar panel voltage

void setup() {
  Serial.begin(9600);
  logger.begin();
  logger.addSensorToList("Solar Panel", "V");
}

void loop() {
   solarPanelVoltage = voltageDividerAdjustment(logger.getNewRawPinReading(analogRead(analogPin), 5, true)); //takes the reading and converts it to a voltage betwen 0.00 and 5.00
   logger.getNewSensorReading("Solar Panel", solarPanelVoltage); //gets new sensor
   logger.logData(true); //logs time down to seconds, change to false if you want only minutes.
   logger.collectionInterval(); //ensures data is collected every five minutes.
}


float voltageDividerAdjustment(float vMeasured) //adjusts voltage output from pin for higher voltage panels, 
{
  return vMeasured / (voltageDividerR2 / (voltageDividerR1 + voltageDividerR2));
}
