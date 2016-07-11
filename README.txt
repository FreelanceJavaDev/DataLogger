This is a library to make a DataLogger.  It requires an RTC and shield with an SD card.
The library configures the SD card, and RTC automatically.  
If the SD card cannot be found there will be an error thrown.
The library is designed to work with no modification to the libraries used.
It is compatible with the Arduino Uno R3 and Mega2560. Mega is reccomended, but not required.  The code was tested and optimized on the Uno.
This library takes up 20KB of program memory and 1.2Kb of SRAM. 512 bytes of that is for the SD card buffer.
The actual SRAM space used by just this library is around 700 bytes.
To use the library import "DataLoggerSD.h" add your sensors using the addSensorToList() function in Setup then use getSensorReading()
For sensors that provide two (or more) readings provide a sensor for each reading.
The inital beta version took 6 weeks of solid 6-8 hour days (occasionaly 10-12 hours) about six days a week.  
Just to get to a releaseable beta version with no bugs.  I did this on my own as a volunteer to help out. 
I'd appreciate the credit and a plug when used.  I use this library myself as you see it here.