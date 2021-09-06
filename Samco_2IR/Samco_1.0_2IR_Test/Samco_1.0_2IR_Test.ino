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
    // modified for use with SAMCO light guns by Sam Ballantyne Nov 2019.
    // modified for http://DFRobot.com by kurakura, Jan 2015.
    // modified for http://DFRobot.com by Lumi, Jan. 2014
    // modified output for Wii-BlobTrack program by RobotFreak http://www.letsmakerobots.com/user/1433
    // Wii Remote IR sensor  test sample code  by kako http://www.kako.com
*/

#include <Wire.h>
#include <DFRobotIRPosition.h>

DFRobotIRPosition myDFRobotIRPosition;         // declare a IRCam object

int positionX[4];               // RAW Sensor Values
int positionY[4];               

int oneY = 0;                   // Re-mapped so left sensor is always read first
int oneX = 0;
int twoY = 0;
int twoX = 0;

int caliPin = A1;               // Set Calibration Pin
int leftPin = A2;               // Set Left Mouse Pin
int rightPin = A3;              // Set Right Mouse Pin
int middlePin = A4;             // Set Middle Mouse Pin
int altPin = A5;                // Set Alt Pin

int caliButton = 0;
int leftButton = 0;
int rightButton = 0;
int middleButton = 0;
int altButton = 0;

int buttonState1 = 0;           // Set Button states
int lastButtonState1 = 0;
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonState3 = 0;
int lastButtonState3 = 0;
int buttonState4 = 0;         
int lastButtonState4 = 0;   
int buttonState5 = 0;         
int lastButtonState5 = 0;  

void setup() {
    pinMode(5, OUTPUT);                     // Needed for IstyBitsy 32u4 5V and other 32u4 boards    
    digitalWrite (5, HIGH);                 // This pin is used to power the IR Camera when using Samco PCB
    delay(500);
    myDFRobotIRPosition.begin();       // initialize the object in setup()
    Serial.begin(9600); 

    pinMode(caliPin, INPUT_PULLUP);         // Set pin modes
    pinMode(leftPin, INPUT_PULLUP);
    pinMode(rightPin, INPUT_PULLUP);
    pinMode(middlePin, INPUT_PULLUP);  
    pinMode(altPin, INPUT_PULLUP);  
}

void loop() {
  
    getPosition();
    mouseButtons();
    altButtons();
    PrintResults();

    
}

void mouseButtons() {    // Setup Cali, Left, Right & Middle Mouse buttons
  
  buttonState1 = digitalRead(caliPin);
  buttonState2 = digitalRead(leftPin);
  buttonState3 = digitalRead(rightPin);
  buttonState4 = digitalRead(middlePin);
      

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      caliButton = 0;
    }
    else { 
      caliButton = 255;
    }
    delay(10);
  }
  
  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      leftButton = 0;
    }
    else { 
      leftButton = 255;
    }
    delay(10);
  }

  if (buttonState3 != lastButtonState3) {
    if (buttonState3 == LOW) {
      rightButton = 0;
    }
    else { 
      rightButton = 255;
    }
    delay(10);
  }

  if (buttonState4 != lastButtonState4) {     
    if (buttonState4 == LOW) {
      middleButton = 0;
    }
    else { 
      middleButton = 255;
    }
    delay(10);
  }
  
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  lastButtonState4 = buttonState4;           
}

void altButtons() {    // Setup Cali, Left, Right & Middle Mouse buttons
  
  buttonState5 = digitalRead(altPin);

    if (buttonState5 != lastButtonState5) {     
    if (buttonState5 == LOW) {
      altButton = 0;
    }
    else { 
      altButton = 255;
    }
    delay(10);
  }
  
  lastButtonState5 = buttonState5;            
}

void PrintResults() {    // Print results for debugging
    Serial.print( oneX );
    Serial.print( "," );
    Serial.print( oneY );
    Serial.print( "," );
    Serial.print( twoX );
    Serial.print( "," );
    Serial.print( twoY );
    Serial.print( "," );
    Serial.print( positionX[2] );
    Serial.print( "," );
    Serial.print( positionY[2] );
    Serial.print( "," );
    Serial.print( positionX[3] );
    Serial.print( "," );
    Serial.print( positionY[3] );
    Serial.print( "," );
    Serial.print( caliButton );
    Serial.print( "," );
    Serial.print( leftButton );
    Serial.print( "," );
    Serial.print( rightButton );
    Serial.print( "," );
    Serial.print( middleButton );
    Serial.print( "," );
    Serial.print( altButton );
    Serial.println();
    delay( 20 );
}

void getPosition() {    // Get tilt adjusted position from IR postioning camera

  myDFRobotIRPosition.requestPosition();

  if (myDFRobotIRPosition.available()) {
    for (int i = 0; i < 4; i++) {
      positionX[i] = myDFRobotIRPosition.readX(i);
      positionY[i] = map (myDFRobotIRPosition.readY(i), 0, 768, 768, 0 );
    }

      if (positionX[0] > positionX[1]) {
      oneY = positionY[0];
      oneX = positionX[0];
      twoY = positionY[1];
      twoX = positionX[1];
    }
    else if (positionX[0] < positionX[1]) {
      oneY = positionY[1];
      oneX = positionX[1];
      twoY = positionY[0];
      twoX = positionX[0];
    }
    else {
      oneY = 1023;
      oneX = 0;
      twoY = 1023;
      twoX = 0;
    }
  }
}
