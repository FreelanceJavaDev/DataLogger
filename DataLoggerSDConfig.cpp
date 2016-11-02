/**
 * @file 		DataLoggerSDConfig.cpp
 * @brief 		Implementation of the Data Logger.
 * @details 	Checks if the SD card has been configured and configures the RTC if needed.\n DO NOT EDIT UNDER ANY CIRCUMSTANCE!
 * @author 		Brian Young
 * @version 	0.9.5 (Beta)
 * @since 		Current: 8/4/16 \n
 * 				Created: 6/21/16
 */

#include "DataLoggerSDConfig.h"

//********* PUBLIC FUNCTIONS START *************

/*!
 * @brief 		Constructs an object for checking the configuration of the SD Shield and RTC.
 * @details		Use of an object is required to use functions within the arduino.
 * @param 		sdPin is the pin used to access the SD card inserted into the shield.
 */
DataLoggerSDConfig::DataLoggerSDConfig(uint8_t sdPin)
{
	_sdPinCheck = sdPin;
}

/*!
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
    check = !RTC.chipPresent();
    #if check
        #error Could not read RTC!
    #endif
    time_t compile_tm = parseClock();
    check = (compile_tm > RTC.get()); //only set RTC if compile time is greater than RTC time
    if(check) 
    {
        if(compile_tm != 0) //USB cable connected
        { RTC.set(compile_tm); }
    }
    setSyncProvider(RTC.get); // the function to get the time from the RTC
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
 * @details Compiler __DATE__ and __TIME__ must be available.
 */
time_t DataLoggerSDConfig::parseClock() {
    int Hour, Min, Sec;
    char month[5];
    int Day, Year;
    tmElements_t tm;
    uint8_t monthNum;
    const char* dateStr = (char*)__DATE__;
    const char* timeStr = (char*)__TIME__;
    
    const char* monthNames = (char *)"JanFebMarAprMayJunJulAugSepOctNovDec";
    //parse time
    if (sscanf(timeStr, "%d:%d:%d", &Hour, &Min, &Sec) != 3){return 0;}
    //parse date
    if (sscanf(dateStr, "%s %d %d", month, &Day, &Year) != 3){return 0;}
    monthNum = (strstr(monthNames, month)-monthNames)/3;
    if (monthNum >= 12){return 0;}
    tm.Hour = Hour;
    tm.Minute = Min;
    tm.Second = Sec;
    tm.Day = Day;
    tm.Month = monthNum + 1;
    tm.Year = CalendarYrToTm(Year);
    
    
    return makeTime(tm);

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
