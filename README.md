# SAMCO - Arduino Powered IR Light Gun

For information on how to use this libraries please visit my YouTube channel https://www.youtube.com/c/samcorulz.

The SAMCO is a Namco light gun with the insides replaced with an Arduino micro controller and DF Robot IR positioning camera and works as HID mouse on LCD (flat screen) TV/monitors.

Gun currently works with Mame in RetroPie (for Raspberry Pi), PSX Beetle (Windows) and PCSX2 (Windows).

Parts:  
Adafruit ItsyBitsy 32u4 (3V or 5V), M0 & M4  
DF Robot IR positioning camera  
Wii USB sensor bar (not dolphin bar, dolphin bar needs wiimote to turn it on)

Features:  
Quick calibration  
Pause/recaibration  
Tilt correction  

In this project I've used libraries from DFRobot & Jonathan Edgecome. Original Libraries can be found at...
https://github.com/DFRobot/DFRobotIRPosition  
https://github.com/jonathanedgecombe/absmouse

UPDATE: There is a great fork of this project for Samco 2.0 boards by Prow7 over at https://github.com/Prow7/ir-light-gun
