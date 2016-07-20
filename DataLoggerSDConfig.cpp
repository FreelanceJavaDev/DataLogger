/**
 * @file 		DataLoggerSDConfig.cpp
 * @brief 		Implementation of the Data Logger.
 * @details 	Checks if the SD card has been configured and configures the RTC if needed.\n DO NOT EDIT UNDER ANY CIRCUMSTANCE!
 * @author 		Brian Young
 * @version 	0.9 (Beta)
 * @since 		Current: 7/2/16 \n
 * 				Created: 6/21/16
 */

#include "DataLoggerSDConfig.h"

//********* PUBLIC FUNCTIONS START *************

/**
 * @brief 		Constructs an object for checking the configuration of the SD Shield and RTC.
 * @details		Use of an object is required to use functions within the arduino.
 * @param 		sdPin is the pin used to access the SD card inserted into the shield.
 */
DataLoggerSDConfig::DataLoggerSDConfig(uint8_t sdPin)
{
	_sdPinCheck = sdPin;
}

/**
 * @brief 		Destructor for the DataLoggerSDConfig. 
 * @details 	This frees memory allocated.
 */
DataLoggerSDConfig::~DataLoggerSDConfig() {} 


/**
 * @brief 		Checks to ensure the SD card and RTC are corretly configured.
 * @details		If the SD card isn't properly configured there will be an error.
 */
void DataLoggerSDConfig::configLogger()
{
	/* SD Card Check START */
	bool check = !SD.begin(_sdPinCheck);
	#if check
			 #error Initialization failed!
	#endif
	/* SD Card Check END */
	/* RTC Check START */
	setSyncProvider(RTC.get);   // the function to get the time from the RTC
	//setSyncInterval(10);
	bool is_time_set = (timeStatus() != timeSet);
	#if is_time_set
		 check = Serial.available();
		#if check 
			time_t t = processSyncMessage();
			# if (t != 0)
				RTC.set(t);   // set the RTC and the system time to the received value
				setTime(t);          
			#endif
		#endif
	#endif
	/* RTC Check END */

}
/*
 * * * * * * * * * * * * * * * * * * *
 * --------------START-------------- *
 * ========PRIVATE FUNCTIONS======== *
 * * * * * * * * * * * * * * * * * * *
 */

/**
 * @brief Configures RTC to the system time and date from the connected computer.
 * @details Serial must be available.
 */
 unsigned long DataLoggerSDConfig::processSyncMessage() {
	const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 
	unsigned long pc_time = 0L;
	if(Serial.find((char*)"T")) {
		 pc_time = Serial.parseInt();
		 if( pc_time < DEFAULT_TIME) {pc_time = 0L;} // check the value is a valid time (greater than Jan 1 2013)
	}
	return pc_time;
}

/*
 * * * * * * * * * * * * * * * * * * *
 * ---------------END--------------- *
 * ========PRIVATE FUNCTIONS======== *
 * * * * * * * * * * * * * * * * * * *
 */

/*
 * * * * * * * * * * * * * * * * * * * 
 *            END OF FILE            *
 * * * * * * * * * * * * * * * * * * *
 */
