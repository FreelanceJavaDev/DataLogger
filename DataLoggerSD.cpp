/*!
 * This is the main DataLogger file. 
 * This code utilizes Structs and Pointers to minimize memory usage. 
 * THIS IS NOT FOR BEGINNERS TO MODIFY!
 * @file DataLoggerSD.cpp
 * @author Brian Young
 * @version 0.9.6 (Beta)
 * @since Current: 9/26/16 \n
 * Created: 6/29/16
 */

#include "DataLoggerSD.h"
/*
 * ------------------------START-----------------------
 * =====================CONSTRUCTORS===================
 */

/*!
 * @brief       The simple version of the DataLogger constructor.  
 * @details     Takes sensor readings every 5 minutes. Interrupt Pin is set to digital pin 2.
 * @param       sdPin is the pin for the SD card defined on the shield.
 */
DataLoggerSD::DataLoggerSD(uint8_t sdPin)
{
    _sdPin = sdPin;
    head = NULL;
    intervalMinutes = DEFUALT_INTERVAL;
    _rtcInterruptPin = 2;
}

/*!
 * @brief      The custom DataLogger constructor.
 * @details    For maximum compatiability select either digital pin 2 or 3 for the interrupt pin.
 * @param      sdPin is the pin for the SD card defined on the shield.
 * @param      minutes defines how many minutes between sensor readings, minimum is one minute.
 * @param      interruptPin is the SQW/SQ pin connected to either digital pin 2 or 3.
 */

DataLoggerSD::DataLoggerSD(uint8_t sdPin, unsigned int minutes, uint8_t interruptPin)
{
    _sdPin = sdPin;
    head = NULL;
    intervalMinutes = minutes;
    
    if(minutes < 1)
        intervalMinutes = 1;
        
    _rtcInterruptPin = interruptPin;
}

/*!
 * @brief 		Destructor for the DataLoggerSD. 
 * @details 	This frees memory allocated.
 */
DataLoggerSD::~DataLoggerSD() {}

/*
 * -------------------------END------------------------
 * =====================CONSTRUCTORS===================
 */

/*
 * ------------------------START-----------------------
 * ===================PUBLIC FUNCTIONS=================
 */

/**
 * @brief       Checks to ensure critical parts are configured correctly.
 * @details     Adds the Date and Time as the first sensor in the data log.
 */
void DataLoggerSD::begin()
{
    DataLoggerSDConfig configCheck(_sdPin);
    configCheck.configLogger();    
    configCheck.~DataLoggerSDConfig();
    addSensorToList("Date", "m/d/yyyy"); //provides Compatible date for spreadsheets
    addSensorToList("Time", "h:mm:ss"); //provides Compatible time for spreadsheets

}

/**
 * @brief       Logs data in a CSV file (Excell compatible), the files are designed to be viewed on a system chronologically.
 * @details     The file name is the current year + the abbreviated month over which the data was collected.
 * @param       includeSeconds is if the log is to include seconds.
 */
void DataLoggerSD::logData(bool includeSeconds)
{
        File monthLog;
        char logFile[18]; //18
        char ascii_year[5];
        strcpy(logFile, itoa(year(), ascii_year) );
        strcat(logFile, (char*)monthShortStr(month()) );
        strcat(logFile, (char*)".csv"); //csv stands for comma seperated value
     if (!SD.exists(logFile))
     {
        monthLog = SD.open(logFile, FILE_WRITE);
            for (SensorInfo* n = head; n; n = n->next) //funky thing is pointers when null they evaluate to false
            {
                monthLog.print(n-> _sensorName);
                monthLog.print('(');
                monthLog.print(n -> _units);
                monthLog.print(')');
                if (n->next) {monthLog.print(DATA_SEPERATOR);} //remove trailing comma
            }

        monthLog.print('\n');
        monthLog.close();
        }
        
        monthLog = SD.open(logFile, FILE_WRITE);
        monthLog.seek(monthLog.size()); //finds the end of the file and goes there to make the log entry
        char* stamp = (char *)malloc(sizeof(char));
        monthLog.print(readDate(stamp));
        free(stamp);
        monthLog.print(DATA_SEPERATOR);
        stamp = (char *)malloc(sizeof(char));
        monthLog.print(readTime(stamp, includeSeconds));
        free(stamp);
        stamp = NULL;
        monthLog.print(DATA_SEPERATOR);
        for ( SensorInfo *n = ((head -> next)->next); n; n = n-> next) //starts after date and time readings
        {
            monthLog.print(n->_sensorReading);
            if ((n->next)) //omitt trailing comma
            { monthLog.print(DATA_SEPERATOR); } 
        }
        
        monthLog.print('\n');
        monthLog.close();
}

/**
 * @brief     Reads date from RTC and converts the numbers to a character string.
 * @details   Date format (all numbers): m/d/yyyy
 * @param     dateStr is the string to hold the date.
 * @return    Date as a string.
 */
char* DataLoggerSD::readDate(char* dateStr) {
    char* date_format = (char*)"/"; 
    char ascii_date[5];
    strcpy(dateStr, itoa(month(), ascii_date) ); 
    strcat(dateStr, date_format);
    strcat(dateStr, itoa(day(), ascii_date) );
    strcat(dateStr, date_format);
    strcat(dateStr, itoa(year(), ascii_date) );
    return dateStr;
}

/**
 * @brief       Reads time from RTC and converts numbers to char*.
 * @details     Time format is in 24-hour format: h:mm:ss.
 * @param       timeStr is the string to hold the time.
 * @param       includeSec includes or skips the seconds if desired.
 * @return      Time as a string.
 */
char* DataLoggerSD::readTime(char* timeStr, bool includeSec) {
    char* time_format = (char*)":";
    char ascii_hour[3];
    char ascii_min[3];
    char ascii_sec[3];
    strcpy(timeStr, itoa(hour(), ascii_hour) );
    strcat(timeStr, time_format);
    strcat(timeStr, timeDigitAdjust(minute(), ascii_min) );
    
    if(includeSec)
    {
        strcat(timeStr, time_format);
        strcat(timeStr, timeDigitAdjust(second(), ascii_sec) );
    }
    return timeStr;
}

/**
 * @brief       This is a placeholder function that will be replaced with a sleep library.
 * @details     Temporary function will be depreciated at a later date.
 */
void DataLoggerSD::collectionInterval()
{
    unsigned long interval = intervalMinutes*MILLIS_PER_MIN;
    unsigned long waitCount = 0;
    while(waitCount < interval)
    {
        delay(1); //this may seem like a waste, but this keeps the RTC accurate
        ++waitCount;
    }
    
}

/**
 * @brief       Gets the reading from a sensor that returns a float and sets it to the given node's readings.
 * @details     If using a pin for raw data use getNewRawPinReading() instead.
 * @param       sensorName is the name of the sensor that gathered the data.
 * @param       sensorReading is the reading taken by the sensor.
 * @see         getNewRawPinReading()
 */
void DataLoggerSD::getNewSensorReading(const char* sensorName, float sensorReading)
{
     SensorInfo *result = search(sensorName);
    if(result)
    { result -> _sensorReading = sensorReading;}
}

/**
 * @brief       Gets the raw pin reading and converts it to a sensor reading.
 * @details     analog will return a voltage, digital returns 1 or 0.
 * @param       pinVal is a value taken from digitalRead() or analogRead()
 * @param       vRef is the voltage refrence, usually either 3.3 or 5.0
 * @param       analog determines the correct value to return.
 * @return      Pin reading, if analog it returns a voltage.
 */
float DataLoggerSD::getNewRawPinReading(int pinVal, float vRef, bool analog)
{
    return analog ? ((pinVal * vRef) / 1023) : (pinVal); //Ternary boolean operation (The C/C++ equivlent of a one line if-else)
}

/**
 * @brief       Creates a new sensor for the Sensor List.
 * @details     Use addSensorToList() instead.
 * @param       sensorName is the name of he sensor.
 * @param       units are units to be used with the sensor.
 * @return      A pointer of a struct of sensor data.
 * @see         addSensorToList()
 */
DataLoggerSD::SensorInfo* DataLoggerSD::addSensor(char* sensorName, char *units)
{
    SensorInfo* addSensor = (SensorInfo *)malloc(sizeof(SensorInfo));
    addSensor -> _sensorName = sensorName;
    addSensor -> _units = units;
    addSensor -> _sensorReading = 0;
    addSensor -> next = NULL;
    return addSensor;
}

/**
 * @brief       Creates a new sensor for the Sensor List and adds it to the end of the list.
 * @details     Order of sensors in list determines order of data in the log file.
 * @param       sensorName is the name of he sensor.
 * @param       units are the desired units to be used with the sensor.
 */
void DataLoggerSD::addSensorToList(const char* sensorName, const char* units)
{
    SensorInfo *node = addSensor((char*)sensorName, (char*)units);
    if(head == NULL)
    {
        head = node;
    }
    else {
    SensorInfo *n = head;
    while (n->next) {
        n = n->next;
    }
    n -> next = node;
    }
}

/**
 * @brief       An alternate method to add a sensor to the list.
 * @details     Advanced user version of addSensorToList(), requires use of addSensor().
 * @param       node is the sensor to be added.
 * @see         addSensor()
 */
void DataLoggerSD::addNodeToList(SensorInfo* node)
{
    if(head == NULL)
    { head = node; }
    
    else 
    {
        SensorInfo *n = head;
        while (n->next) { n = n->next;}
        n -> next = node;
    }
}

/*
 * -------------------------END------------------------
 * ===================PUBLIC FUNCTIONS=================
 */

/*
 * ------------------------START------------------------
 * ===================PRIVATE FUNCTIONS=================
 */

/**
 * @brief       A search utility function to find a sensor in a list.
 * @details     Used inside of other functions in this class.
 * @param       senseName is the name of the sensor to be searched.
 * @return      The sensor node that matches the parameter if found, NULL otherwise.
 */
DataLoggerSD::SensorInfo* DataLoggerSD::search(const char* senseName)
{
    SensorInfo *n = head;
    
    while(strcmp((n->_sensorName), senseName) != 0)
    {
        if ((n -> next) == NULL)
        {
            return NULL;
        }
        n = n-> next;
    }
    return n;
}

/**
 * @brief       A utility that converts base 10 int to ascii.
 * @details     Used with converting time and date.
 * @param       val is int to be turned into string.
 * @param       buff is a buffer to hold the converted int.
 * @return      A string representation of provided int value.
 */
char* DataLoggerSD::itoa(int val, char* buff)
{
    uint8_t placeCount = 0;
    int temp = val;
    while (temp > 0)
    {
        temp /= 10;
        ++placeCount;
    }
    
    uint8_t lastIndex = placeCount - 1;
    placeCount = 0;
    while (val > 0)
    {
        buff[lastIndex - placeCount] = ((val % 10) + '0');
        val /= 10;
        ++placeCount;
    }
     
    buff[lastIndex+1] ='\0'; 
    return buff;
}

/**
 * @brief       Utility function for readTime.  Is an int to prevent warning when doing an explicit cast.
 * @details     There was no difference in memory usage.
 * @param       digits is the number to be converted.
 * @param       buff is the character buffer to put the digits into.
 * @return      Char pointer to resulting number.
 */
char* DataLoggerSD::timeDigitAdjust(int digits, char* buff) {
    if (digits < 10)
    {
        buff[0] = '0';
        buff[1] = (digits+'0');
        buff[2] = '\0';
        return buff;
    }
        
    else { return itoa(digits, buff); }    
}

/*
 * -------------------------END-------------------------
 * ===================PRIVATE FUNCTIONS=================
 */

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *								END OF FILE								*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */	