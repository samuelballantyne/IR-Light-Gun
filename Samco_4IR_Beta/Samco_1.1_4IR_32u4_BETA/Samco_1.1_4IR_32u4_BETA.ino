/*!
 * @file Samco_1.1_4IR_BETA.ino
 * @brief 4 Button Light Gun sketch for 4 LED setup
 * @n INO file for Samco Light Gun 4 LED setup
 *
 * @copyright   Samco, https://github.com/samuelballantyne, May 2021
 * @copyright   GNU Lesser General Public License
 *
 * @author [Sam Ballantyne](samuelballantyne@hotmail.com)
 * @version  V1.0
 * @date  2021
 * 
 */


/* HOW TO CALIBRATE:
 *  
 *  Step 1: Push Calibration Button
 *  Step 2: Pull Trigger
 *  Step 3: Shoot Center of the Screen (try an do this as acuratley as possible)
 *  Step 4: Mouse should lock to vertiucal axis, use A/B buttons to adjust mouse up/down
 *  Step 5: Pull Trigger
 *  Step 6: Mouse should lock to horizontal axis, use A/B buttons to adjust mouse left/right
 *  Step 7: Pull Trigger to finish
 *  Step 8: Offset are now saved to EEPROM
*/


int xCenter = 500;              // Open serial monitor and update these values to save calibration
int yCenter = 450;
float xOffset = 145;             
float yOffset = 80;

int finalX = 0;                 // Values after tilt correction
int finalY = 0;

int MoveXAxis = 0;              // Unconstrained mouse postion
int MoveYAxis = 0;               

int conMoveXAxis = 0;           // Constrained mouse postion
int conMoveYAxis = 0;           

int count = -2;                  // Set intial count

int caliPin = 11;               // Set Calibration Pin (change to A4 to use ALT Pin)
int leftPin = 12;               // Set Left Mouse Pin
int rightPin = 7;               // Set Right Mouse Pin
int middlePin = 9;              // Set Middle Mouse Pin
int pedalPin = A1;              // Set Pedal Pin

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

int plus = 0;         
int minus = 0; 

#include <HID.h>                // Load libraries
#include <Wire.h>
#include <AbsMouse.h>
#include <DFRobotIRPosition.h>
#include <SamcoBeta.h>
#include <EEPROM.h>

DFRobotIRPosition myDFRobotIRPosition;
SamcoBeta mySamco;

int res_x = 1023;              // UPDATE: These values do not need to change
int res_y = 768;               // UPDATE: These values do not need to change


void setup() {

  pinMode(5, OUTPUT);                     // Needed for IstyBitsy 32u4 5V and other 32u4 boards    
  digitalWrite (5, HIGH);                 // This pin is used to power the IR Camera when using Samco PCB
  delay(500);
  
  Serial.begin(115200);                     // For saving calibration (make sure your serial monitor has the same baud rate)

  loadSettings();
  
  AbsMouse.init(res_x, res_y);            

  pinMode(caliPin, INPUT_PULLUP);         // Set pin modes
  pinMode(leftPin, INPUT_PULLUP);
  pinMode(rightPin, INPUT_PULLUP);
  pinMode(middlePin, INPUT_PULLUP); 
  pinMode(pedalPin, INPUT_PULLUP);      

  myDFRobotIRPosition.begin();            // Start IR Camera

  AbsMouse.move((res_x / 2), (res_y / 2));          // Set mouse position to centre of the screen
  
  delay(500);
  
}




void loop() {


  /* ------------------ START/PAUSE MOUSE ---------------------- */


  if (count > 3 ) {

    skip();
    mouseCount();
    PrintResults();

  }


  /* ---------------------- CENTRE --------------------------- */


  else if (count > 2 ) {

    AbsMouse.move((res_x / 2), (res_y / 2));

    mouseCount();
    getPosition();

    xCenter = finalX;
    yCenter = finalY;

    PrintResults();

  }


  /* -------------------- OFFSET ------------------------- */


  else if (count > 1 ) {

    mouseCount();
    AbsMouse.move(conMoveXAxis, conMoveYAxis);
    getPosition();

    MoveYAxis = map (finalY, (yCenter + ((mySamco.H() * (yOffset / 100)) / 2)), (yCenter - ((mySamco.H() * (yOffset / 100)) / 2)), 0, res_y);
    conMoveXAxis = res_x/2;
    conMoveYAxis = constrain (MoveYAxis, 0, res_y);
    
    if (plus == 1){
    yOffset = yOffset + 1;
    delay(10);
    } else {
      }

    if (minus == 1){
    yOffset = yOffset - 1;
    delay(10);
    } else {
      }
      
    PrintResults();

  }

  
  else if (count > 0 ) {

    mouseCount();
    AbsMouse.move(conMoveXAxis, conMoveYAxis);
    getPosition();

    MoveXAxis = map (finalX, (xCenter + ((mySamco.H() * (xOffset / 100)) / 2)), (xCenter - ((mySamco.H() * (xOffset / 100)) / 2)), 0, res_x);
    conMoveXAxis = constrain (MoveXAxis, 0, res_x);
    conMoveYAxis = res_y/2;
    
    if (plus == 1){
    xOffset = xOffset + 1;
    delay(10);
    } else {
      }

    if (minus == 1){
    xOffset = xOffset - 1;
    delay(10);
    } else {
      }
      
    PrintResults();

  }

  else if (count > -1) {
    
    count = count - 1;
    
    EEPROM.write(0, xCenter - 256);
    EEPROM.write(1, yCenter - 256);
    EEPROM.write(2, xOffset);
    EEPROM.write(3, yOffset);
  }


  /* ---------------------- LET'S GO --------------------------- */


  else {

    AbsMouse.move(conMoveXAxis, conMoveYAxis);

    mouseButtons();
    getPosition();

    MoveXAxis = map (finalX, (xCenter + ((mySamco.H() * (xOffset / 100)) / 2)), (xCenter - ((mySamco.H() * (xOffset / 100)) / 2)), 0, res_x);
    MoveYAxis = map (finalY, (yCenter + ((mySamco.H() * (yOffset / 100)) / 2)), (yCenter - ((mySamco.H() * (yOffset / 100)) / 2)), 0, res_y);
    conMoveXAxis = constrain (MoveXAxis, 0, res_x);
    conMoveYAxis = constrain (MoveYAxis, 0, res_y);
    
    PrintResults();
    reset();
//  footPedal();         // Uncomment to use foot pedal attached to pin A1

  }

}


/*        -----------------------------------------------        */
/* --------------------------- METHODS ------------------------- */
/*        -----------------------------------------------        */


void getPosition() {    // Get tilt adjusted position from IR postioning camera

myDFRobotIRPosition.requestPosition();
    if (myDFRobotIRPosition.available()) {
    mySamco.begin(myDFRobotIRPosition.readX(0), myDFRobotIRPosition.readY(0), myDFRobotIRPosition.readX(1), myDFRobotIRPosition.readY(1),myDFRobotIRPosition.readX(2), myDFRobotIRPosition.readY(2),myDFRobotIRPosition.readX(3), myDFRobotIRPosition.readY(3), xCenter, yCenter);
    finalX = mySamco.X();
    finalY = mySamco.Y();
    }
    else {
    Serial.println("Device not available!");
    }
}


void go() {    // Setup Start Calibration Button

  buttonState1 = digitalRead(caliPin);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      count--;
    }
    else { // do nothing
    }
    delay(50);
  }
  lastButtonState1 = buttonState1;
}


void mouseButtons() {    // Setup Left, Right & Middle Mouse buttons

  buttonState2 = digitalRead(leftPin);
  buttonState3 = digitalRead(rightPin);
  buttonState4 = digitalRead(middlePin);     

  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      AbsMouse.press(MOUSE_LEFT);
    }
    else {
      AbsMouse.release(MOUSE_LEFT);
    }
    delay(10);
  }

  if (buttonState3 != lastButtonState3) {
    if (buttonState3 == LOW) {
      AbsMouse.press(MOUSE_RIGHT);
    }
    else {
      AbsMouse.release(MOUSE_RIGHT);
    }
    delay(10);
  }

  if (buttonState4 != lastButtonState4) {     
    if (buttonState4 == LOW) {
      AbsMouse.press(MOUSE_MIDDLE);
    }
    else {
      AbsMouse.release(MOUSE_MIDDLE);
    }
    delay(10);
  }

  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  lastButtonState4 = buttonState4;            
}


void footPedal() {    // Foot pedal works as and extra right mouse buutton

  buttonState5 = digitalRead(pedalPin);     

  if (buttonState5 != lastButtonState5) {     
    if (buttonState5 == LOW) {
      AbsMouse.press(MOUSE_RIGHT);
    }
    else {
      AbsMouse.release(MOUSE_RIGHT);
    }
    delay(10);
  }

  lastButtonState5 = buttonState5;            
}


void mouseCount() {    // Set count down on trigger

  buttonState2 = digitalRead(leftPin);
  buttonState3 = digitalRead(middlePin);
  buttonState4 = digitalRead(rightPin);     

  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      count--;
    }
    else {
    }
    delay(10);
  }

  if (buttonState3 != lastButtonState3) {
    if (buttonState3 == LOW) {
      plus = 1;
    }
    else {
      plus = 0;
    }
    delay(10);
  }

  if (buttonState4 != lastButtonState4) {     
    if (buttonState4 == LOW) {
      minus = 1;
    }
    else {
      minus = 0;
    }
    delay(10);
  }

  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  lastButtonState4 = buttonState4;            
}


void reset() {    // Pause/Re-calibrate button

  buttonState1 = digitalRead(caliPin);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      count = 4;
      delay(50);
    }
    else { // do nothing
    }
    delay(50);
  }
  lastButtonState1 = buttonState1;
}


void skip() {    // Unpause button

  buttonState1 = digitalRead(caliPin);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      count = 0;
      delay(50);
    }
    else { // do nothing
    }
    delay(50);
  }
  lastButtonState1 = buttonState1;
}


void loadSettings() {
  if (EEPROM.read(1023) == 'T') {
    //settings have been initialized, read them
    xCenter = EEPROM.read(0) + 256;
    yCenter = EEPROM.read(1) + 256;
    xOffset = EEPROM.read(2);
    yOffset = EEPROM.read(3);
  } else {
    //first time run, settings were never set
    EEPROM.write(0, xCenter - 256);
    EEPROM.write(1, yCenter - 256);
    EEPROM.write(2, xOffset);
    EEPROM.write(3, yOffset);
    EEPROM.write(1023, 'T');    
  }
}


void PrintResults() {    // Print results for saving calibration

  Serial.print("CALIBRATION:");
  Serial.print("     Cam Center x/y: ");
  Serial.print(xCenter);
  Serial.print(", ");
  Serial.print(yCenter);
  Serial.print("     Offsets x/y: ");
  Serial.print(xOffset);
  Serial.print(", ");
  Serial.println(yOffset);

}
