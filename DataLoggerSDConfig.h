/**
 * @file 		DataLoggerSDConfig.h
 * @brief 		Used to Configure the Data Logger.
 * @details 	Checks if the SD card has been configured and configures the RTC if needed.
 * @author 		Brian Young
 * @version 	0.9 (Beta)
 * @since 		Current: 7/2/16 \n
 * 				Created: 6/21/16
 */

#ifndef _DataLoggerSDConfig_h_
#define _DataLoggerSDConfig_h_

#include <SD.h>
#include <Time.h>
#include <DS1307RTC.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class DataLoggerSDConfig {
	public:
		//functions
		DataLoggerSDConfig(uint8_t sdPin);
		~DataLoggerSDConfig(); //destructor, frees memory allocated.
		void configLogger();

	private:
	//funtions
		unsigned long processSyncMessage();
	//data
		/**
		* @var _sdPinCheck 
		* @brief
		* @details  The pin used to access the SD card by the shield.
		*/
		uint8_t _sdPinCheck;
};
#endif