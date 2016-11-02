/**
 * @file 		DataLoggerSDConfig.h
 * @brief 		Used to Configure the Data Logger.
 * @details 	Checks if the SD card has been configured and configures the RTC if needed.
 * @author 		Brian Young
 * @version 	0.9.5 (Beta)
 * @since 		Current: 8/4/16 \n
 * 				Created: 6/21/16
 */

#ifndef _DataLoggerSDConfig_h_
#define _DataLoggerSDConfig_h_
#include <SD.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <time.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifndef __AVR__
#include <sys/types.h> // for __time_t_defined, but avr libc lacks sys/types.h
#endif

class DataLoggerSDConfig {
	public:
		//constructors
		DataLoggerSDConfig(uint8_t sdPin);
		~DataLoggerSDConfig(); //destructor, frees memory allocated.
		
		//functions
		void configLogger();

	private:
	//functions
		time_t parseClock();

	//data
		uint8_t _sdPinCheck; /*!< The pin used to access the SD card by the shield. */
};
#endif