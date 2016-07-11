/**
 * Helper library that checks the SD card and RTC are configured correctly.
 * DO NOT EDIT UNDER ANY CIRCUMSTANCE!
 * @class DataLoggerSDConfig \headerfile
 * @author Brian Young
 * @version 0.9 (Beta)
 * @since Current: 7/2/16 \n
 * Created: 6/21/16
 */

#include "DataLoggerSDConfig.h"

//********* PUBLIC FUNCTIONS START *************

/**
 * Constructor for checking the configuration of the SD Shield and RTC.
 * @param sdPin is the pin used to access the SD card inserted into the shield.
 */
DataLoggerSDConfig::DataLoggerSDConfig(uint8_t sdPin)
{
	_sdPinCheck = sdPin;
}

/**
 * The destructor for the DataLoggerSDConfig. 
 * This frees memory allocated.
 */
DataLoggerSDConfig::~DataLoggerSDConfig() {} 


/**
 * Checks the setup to ensure the SD card and RTC are corretly configured.
 * If the SD card isn't properly configured there will be an error.
 */
void DataLoggerSDConfig::configLogger()
{
	/* SD Card Check START */
	bool check = !SD.begin(_sdPinCheck);
	#if check
			 #error Initialization failed!
	#endif
	/* SD Card Check END */
	//formatSD();
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
	//****** PRIVATE FUNCTIONS START *******
	
	/**
	 * Formats SD card to follow a format.
	 * Adds a Log Folder if the directory doesn't exit.
	 * @deprecated
	 */
	void DataLoggerSDConfig::formatSD()
	{
		if(!SD.exists(LOG_DIR))
			SD.mkdir(LOG_DIR);
	}
 
/**
 * Retrieves the system time and date from the connected computer.
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
