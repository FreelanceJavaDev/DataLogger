/**
 * Helper library that checks the SD card and RTC are configured correctly.
 * DO NOT EDIT UNDER ANY CIRCUMSTANCE 
 * @file DataLoggerSDConfig.h
 * @author Brian Young
 * @version 0.9 (Beta)
 * @since Current: 7/2/16 \n
 * Created: 6/21/16
 */
#include <Arduino.h>
#include <SD.h>
#include <Time.h>
#include <DS1307RTC.h>

#ifndef _DataLoggerSDConfig_h_
#define _DataLoggerSDConfig_h_
#define LOG_DIR (char*)"DATA" /**< @deprecated Log Directory.**/
class DataLoggerSDConfig {
	public:
		//functions
		DataLoggerSDConfig(uint8_t sdPin);
		~DataLoggerSDConfig(); //destructor, frees memory allocated.
		void configLogger();
		//data

	private:
	//funtions
		void formatSD();
		unsigned long processSyncMessage();
	//data
		uint8_t _sdPinCheck; /** < SD pin to check.*/
};
#endif