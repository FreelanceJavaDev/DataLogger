/**
 * @header 		DataLoggerSD.h
 * @short 		Contains declarations for Data Logging functions.
 * @details		This code utilizes Structs and Pointers to minimize memory usage.
 * 				THIS IS NOT FOR BEGINNERS TO MODIFY!
 * @author 		Brian Young
 * @version 	0.9.5 (Beta)
 * @since 		Current: 8/4/16\n
 * 				Created: 6/29/16
 */
#ifndef _DataLoggerSD_h_
#define _DataLoggerSD_h_

#include <SPI.h>
#include <SD.h>
#include <Time.h>
#include <DS1307RTC.h>
#include "DataLoggerSDConfig.h" /** @see DataLoggerSDConfig.h#*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) //Arduino UNO
	#define SDA A4
	#define SCL A5
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) //Arduino MEGA
	#define SDA 20
	#define SCL 21
#else //Everything else
	#error Arduino Board is incompatible or obsolete.
#endif

#define MILLIS_PER_MIN (unsigned long)60000 /*! @define MILLIS_PER_MIN 		The number of milisecons per minute (60,000).*/
#define DEFUALT_INTERVAL (uint8_t)5 /*! @define DEFUALT_INTERVAL			5 minute defualt sensor data interval.*/
#define DATA_SEPERATOR ',' /*! @define 			DATA_SEPERATOR 				Seperates data in the log file.*/

class DataLoggerSD {
		public:
			/**
			 * @struct		SensorInfo
			 * @brief	 	All nessicary information about a sensor is stored in here.
			 * @details 	SensorInfo is used used to create a linked list of sensors.
			 */
			typedef struct SensorInfo {
				char *_sensorName; /*!< The provided name of the sensor.*/
				char *_units; /*!< The units used for the sensor's readings.*/
				float _sensorReading; /*!< The most recent sensor reading taken.*/
				SensorInfo *next; /*!< Refrence to next sensor in the list.*/
			} SensorInfo; /*!< Defines a type for struct SensorInfo.*/
			
			//constructors
			DataLoggerSD(uint8_t sdPin);
			DataLoggerSD(uint8_t sdPin, unsigned int minutes, uint8_t interruptPin);
			~DataLoggerSD(); //Destructor
			
			//functions
			void begin(void);
			void getNewSensorReading(const char* sensorName, float sensorReading);
			float getNewRawPinReading(int pinVal, float vRef, bool analog);
			SensorInfo* addSensor(char* sensorName, char *units);
			void addSensorToList(const char* sensorName, const char *units);
			void addNodeToList(SensorInfo* node);
			void logData(bool includeSeconds);
			char* readDate(char* dateStr);
			char* readTime(char* timeStr, bool includeSec);
			void collectionInterval(void);

		private:
			//functions
			char* timeDigitAdjust(int digits, char* buff);
			SensorInfo* search(const char* senseName);
			char* itoa(int val, char* buff);
			
			//data
			uint8_t _sdPin; /*!< The pin that accesses the SD card.*/
			SensorInfo* head; /*!< The first sensor in the Sensor list.*/
			uint8_t _rtcInterruptPin; /*!< RTC interrupt pins are digital pin 2 and 3 on Arduino UNO and MEGA boards. */
			//bool _sleepEnabled;
			unsigned int intervalMinutes; /*!< Minutes between log entries.*/
	};
#endif