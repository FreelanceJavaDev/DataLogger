/**
 * The main Data Logger .
 * This code utilizes Structs and Pointers to minimize memory usage. 
 * THIS IS NOT FOR BEGINNERS TO MODIFY!
 * @file DataLoggerSD.h
 * @author Brian Young
 * @version 0.9 (Beta)
 * @since Current: 7/10/16\n
 * Created: 6/29/16
 */
#include <Arduino.h>
#include <SD.h>
#include <Time.h>
#include <DS1307RTC.h>
#include "DataLoggerSDConfig.h" /**< @see DataLoggerSDConfig*/
#ifndef _DataLoggerSD_h_
#define _DataLoggerSD_h_
#define MILLIS_PER_MIN (unsigned long)60000 /**< constant 1000 milis per second 60 seconds per min.*/
#define DEFUALT_INTERVAL (uint8_t)5 /**< 5 minute sensor logging interval*/
#define DATA_SEPERATOR ',' /**< Data seperater for log file.*/

class DataLoggerSD {
		public:
			/**
			 * SensorInfo defines a type used to create a linked list.
			 */
			typedef struct SensorInfo {
				char *_sensorName; /**< Sensor name.*/
				char *_units; /**< Sensor units.*/
				float _sensorReading; /**< Current sensor reading.*/
				SensorInfo *next; /**< Next sensor in list.*/
			} SensorInfo;
			//functions
			/**
			 * The simple DataLogger Constructor.  
			 * It takes sensor readings every 5 minutes.
			 * Interrupt Pin is set to digital pin 2.
			 * @param sdPin is the pin for the SD card defined on the shield.
			 */
			DataLoggerSD(uint8_t sdPin);
			DataLoggerSD(uint8_t sdPin, unsigned int minutes, uint8_t interruptPin);
			void begin();
			void getNewSensorReading(const char* sensorName, float sensorReading);
			float getNewRawPinReading(int pinVal, float vRef, bool analog);
			SensorInfo* addSensor(char* sensorName, char *units);
			void addSensorToList(const char* sensorName, const char *units);
			void addNodeToList(SensorInfo* node);
			void logData(bool includeSec);
			char* readDate(char* date_str);
			char* readTime(char* time_str, bool includeSec);
			void collectionInterval();

		private:
			//utility functions
			char* timeDigitAdjust(int digits, char* buff);
			SensorInfo* search(const char* senseName);
			char* itoa(int val, char* buff);
			//data
			uint8_t _sdPin;
			SensorInfo* head;
			//uint8_t _rtcInterruptPin; //RTC interrupt pin is either digital pin 2 or 3 on arduino boards
			//bool _sleepEnabled;
			unsigned int interval_minutes; //Assuming the sleep for minutes and a reading is beign taken at least once a day.

	};
#endif