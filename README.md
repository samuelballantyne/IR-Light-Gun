# SAMCO - Arduino Powered IR Light Gun

For information on how to use this library please visit my YouTube channel https://www.youtube.com/channel/UCBKlb8pC4SFoUV84F8EHSLw.

The SAMCO is a Namco light gun with the insides replaced with an Arduino micro controller and DF Robot IR positioning camera and works as HID mouse on LCD (flat screen) TV/monitors.

Gun currently works with Mame in RetroPie (for Raspberry Pi), PSX Beetle (Windows) and PCSX2 (Windows).

Parts:  
Adafruit ItsyBitsy M0 (code will work with any HID supported microcontroller with a slight adjustment)  
DF Robot IR positioning camera  
2 LED's  
2 100 ohm Resistors  

Features:  
Quick calibration  
Pause/recaibration  
Tilt correction  

In this project I've used libraries from DFRobot & Jonathan Edgecome. Original Libraries can be found at...
https://github.com/DFRobot/DFRobotIRPosition  
https://github.com/jonathanedgecombe/absmouse
