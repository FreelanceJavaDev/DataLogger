#Data Logger Library
#### Author

* [Brian Young](https://github.com/FreelanceJavaDev)

## Supported Devices

* Uno - Tested
* Mega - In progress

## Supported Shields
* SD Card Shield - Tested
* Adafruit 1.8" TFT shield with Micro SD card slot - In Progress
* Ethernet Shield - Untested, but likely compatable.

## Introduction
I did this on my own as a volunteer, I made no money on this nor do I plan to. 
I'd appreciate the credit and a plug when used.  I use this library myself as you see it here.  Feedback and bug reports are greatly appreciated, but fixes may take time depending on what else I have going on.
In additon the standrd RTC1307 library (which does DS1302, DS1307 and DS2302 RTCs) and the Time sibrary are required. 
Both can be obtained through the Sketch -> Include Library -> Manage Libraries menu item.

## Overview 
This is a library to make a DataLogger with an Arduino using the standard Arduino IDE.  It requires an RTC and shield with an SD card to work.
The library configures the SD card and RTC automatically upon compilation and/or upload to an Arduino.  There is no GUI even if the Shield has a screen. 

## Details
If the SD card cannot be found there will be an error thrown.
The library is designed to work with no modification to the libraries used.  Additional Sensor Libraries may be added to the Arduino Sketch.
It is compatible with the Arduino Uno R3 and Mega2560. Mega is reccomended, but not required.  The code was tested and optimized on the Arduino Uno.
This library takes up 22KB of program memory and 1.2Kb of SRAM. 512 bytes of that is for the SD card buffer.
The actual SRAM space used by this library is around 700 bytes.

## To Use
To use the library import "DataLoggerSD.h" add your sensors using the addSensorToList() function in Setup then use getSensorReading()
For sensors that provide two (or more) readings provide a sensor for each reading.

## Time to releaseable Beta version and likely bug fixes
The inital beta version took 6 weeks of solid 6-8 hour days (occasionaly 10-12 hours) about six days a week.  
Just to get to a releaseable beta version. I am just one person and I don't dedicate every waking hour to updating code I've released. Bug fixes may take some time.


## Upcoming planned features
The current library does not include an optimized and embedded sleep libary.  I will be adding an embedded sleep library at some point.
