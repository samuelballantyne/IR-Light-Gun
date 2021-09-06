/*
    This is an example Arduino code for DFRobot Positioning Infrared Camera ( SKU: SEN0158 ).
    Basically, it's a camera that can "see" infrared lights and ascertain the coordinates of those lights, just like what's inside a Wiimote.
    It can track up to four infrared lights at a time.
    You can find more info on this sensor at https://www.dfrobot.com/index.php?route=product/product&product_id=1088
    
    WHAT THIS SKETCH DOES:
    Outputs x and y coordinate values for four points to Serial out.
    When a point is not detected ( blocked, out of range, etc ), the sensor outputs 1023 for both x and y values.
    
    HOOKUP:   
            Arduino     Positioning IR Camera
    ===============     =====================
                 5V --> Red wire
                GND --> Black wire
    SDA (A4 on UNO) --> Yellow wire
    SCL (A5 on UNO) --> Green wire
    
    MODIFICATION HISTORY:
    // modified for use with SAMCO light guns 4 Led setup by Sam Ballantyne April 2020.
    // modified for http://DFRobot.com by kurakura, Jan 2015.
    // modified for http://DFRobot.com by Lumi, Jan. 2014
    // modified output for Wii-BlobTrack program by RobotFreak http://www.letsmakerobots.com/user/1433
    // Wii Remote IR sensor  test sample code  by kako http://www.kako.com
*/

#include <Wire.h>
#include <Samco.h>
#include <DFRobotIRPosition.h>

DFRobotIRPosition myDFRobotIRPosition;
Samco mySamco;

int rawX[4];               // RAW Sensor Values
int rawY[4];

int xCenter = 512;
int yCenter = 368;

int mouseX;
int mouseY;
int medianX;
int medianY;

int time;

void setup() {
    
    pinMode(5, OUTPUT);                // Needed for IstyBitsy 32u4 5V and other 32u4 boards    
    digitalWrite (5, HIGH);            // Only required for SAMCO 1.1
    delay(500);

    myDFRobotIRPosition.begin();       // initialize the object in setup()
    Serial.begin(9600); 
    
}

void loop() {
  
    getPosition();
    PrintResults();
    
}

void PrintResults() {    // Print results for debugging
    Serial.print( rawX[0] + 100);
    Serial.print( "," );
    Serial.print( rawY[0] + 100);
    Serial.print( "," );
    Serial.print( rawX[1] + 100);
    Serial.print( "," );
    Serial.print( rawY[1] + 100);
    Serial.print( "," );
    Serial.print( rawX[2] + 100);
    Serial.print( "," );
    Serial.print( rawY[2] + 100);
    Serial.print( "," );
    Serial.print( rawX[3] + 100);
    Serial.print( "," );
    Serial.print( rawY[3] + 100);
    Serial.print( "," );
    Serial.print( mouseX + 100);
    Serial.print( "," );
    Serial.print( mouseY + 100);
    Serial.print( "," );
    Serial.print( medianX + 100);
    Serial.print( "," );
    Serial.print( medianY + 100);
    Serial.println();
}

void getPosition() {    // Get tilt adjusted position from IR postioning camera


myDFRobotIRPosition.requestPosition();
    if (myDFRobotIRPosition.available()) {
    mySamco.begin(myDFRobotIRPosition.readX(0), myDFRobotIRPosition.readY(0), myDFRobotIRPosition.readX(1), myDFRobotIRPosition.readY(1),myDFRobotIRPosition.readX(2), myDFRobotIRPosition.readY(2),myDFRobotIRPosition.readX(3), myDFRobotIRPosition.readY(3), xCenter, yCenter);
    for (int i = 0; i < 4; i++) {
      rawX[i] = mySamco.testX(i);
      rawY[i] = mySamco.testY(i);
    }
    mouseX = mySamco.X();
    mouseY = mySamco.Y();
    medianX = mySamco.testMedianX();
    medianY = mySamco.testMedianY();
    time = micros();
    }
    else {
    Serial.println("Device not available!");
    }
}

/* <----- It is common for cameras to be labeled/installed upside down. 
 *        This has no impact on the final sketch but will cause test sketch to be inverted.
 *        If this is the case use remapped sketch below.

myDFRobotIRPosition.requestPosition();
    if (myDFRobotIRPosition.available()) {
    mySamco.begin(myDFRobotIRPosition.readX(0), myDFRobotIRPosition.readY(0), myDFRobotIRPosition.readX(1), myDFRobotIRPosition.readY(1),myDFRobotIRPosition.readX(2), myDFRobotIRPosition.readY(2),myDFRobotIRPosition.readX(3), myDFRobotIRPosition.readY(3), xCenter, yCenter);
    for (int i = 0; i < 4; i++) {
      rawX[i] = map(mySamco.testX(i), 0, 1023, 1023, 0);
      rawY[i] = map(mySamco.testY(i), 0, 768, 768, 0);
    }
    mouseX = map(mySamco.X(), 0, 1023, 1023, 0);
    mouseY = map(mySamco.Y(), 0, 768, 768, 0);
    centerX = map(mySamco.testMedianX(), 0, 1023, 1023, 0);
    centerY = map(mySamco.testMedianY(), 0, 768, 768, 0);
    }
    else {
    Serial.println("Device not available!");
    }
}

*/
