/*

  Samco - Arduino Powered IR Light Gun

  Go to https://github.com/samuelballantyne for code & instructions.

  created June 2019
  by Sam Ballantyne

  @copyright   Samco, https://github.com/samuelballantyne, October 2021
  @copyright   GNU Lesser General Public License

*/

int positionX[4];               // RAW Sensor Values
int positionY[4];               

int oneY = 0;                   // Re-mapped so left sensor is always read first
int oneX = 0;
int twoY = 0;
int twoX = 0;

int finalX = 0;                 // Values after tilt correction
int finalY = 0;

int xLeft = 0;                  // Stored calibration points
int yTop = 0;
int xRight = 0;
int yBottom = 0;

int MoveXAxis = 0;              // Unconstrained mouse postion
int MoveYAxis = 0;               

int conMoveXAxis = 0;           // Constrained mouse postion
int conMoveYAxis = 0;           

int count = 4;                  // Set intial count

int _tigger = 7;               // Label Pin to buttons
int _up = 11;                
int _down = 9;              
int _left = 10;             
int _right = 12;               
int _APin = A1;                
int _BPin = A0;              
int _startPin = A2; 
int _select = A3;               
int _reload = 13;
int _pedal = 5;               


int buttonState1 = 0;           
int lastButtonState1 = 0;
int buttonState2 = 0;
int lastButtonState2 = 0;
int buttonState3 = 0;
int lastButtonState3 = 0;
int buttonState4 = 0;         
int lastButtonState4 = 0; 
int buttonState5 = 0;           
int lastButtonState5 = 0;
int buttonState6 = 0;
int lastButtonState6 = 0;
int buttonState7 = 0;
int lastButtonState7 = 0;
int buttonState8 = 0;         
int lastButtonState8 = 0;          
int buttonState9 = 0;           
int lastButtonState9 = 0;
int buttonState10 = 0;
int lastButtonState10 = 0;    

#include <HID.h>                // Load libraries
#include <Wire.h>
#include <Keyboard.h>
#include <AbsMouse.h>
#include <DFRobotIRPosition.h>

DFRobotIRPosition myDFRobotIRPosition;

int res_x = 1920;               // Put your screen resolution width here
int res_y = 1080;               // Put your screen resolution height here


void setup() {

  Serial.begin(9600);                     // For debugging (make sure your serial monitor has the same baud rate)

  AbsMouse.init(res_x, res_y);            

  pinMode(_tigger, INPUT_PULLUP);         // Set pin modes
  pinMode(_up, INPUT_PULLUP);
  pinMode(_down, INPUT_PULLUP);
  pinMode(_left, INPUT_PULLUP);
  pinMode(_right, INPUT_PULLUP);         // Set pin modes
  pinMode(_APin, INPUT_PULLUP);
  pinMode(_BPin, INPUT_PULLUP);
  pinMode(_startPin, INPUT_PULLUP);  
  pinMode(_select, INPUT_PULLUP);
  pinMode(_reload, INPUT_PULLUP);       

  myDFRobotIRPosition.begin();            // Start IR Camera

  AbsMouse.move((res_x / 2), (res_y / 2));          // Set mouse position to centre of the screen
  
  delay(500);
  
}




void loop() {

  if (count > 3) {

    getPosition();
    mouseButtons();
    PrintResults();
    go();

  }

  /* ------------------ START/PAUSE MOUSE ---------------------- */


  else if (count > 2 ) {

    skip();
    mouseCount();
    mouseButtons();
    PrintResults();

  }


  /* ---------------------- TOP LEFT --------------------------- */


  else if (count > 1 ) {

    AbsMouse.move(300, 200);

    mouseCount();
    getPosition();
    reset();

    xLeft = finalX;
    yTop = finalY;

    PrintResults();

  }


  /* -------------------- BOTTOM RIGHT ------------------------- */


  else if (count > 0 ) {

    AbsMouse.move((res_x - 300), (res_y - 200));

    mouseCount();
    getPosition();
    reset();

    xRight = finalX;
    yBottom = finalY;

    PrintResults();

  }


  /* ---------------------- LET'S GO --------------------------- */


  else {

    AbsMouse.move(conMoveXAxis, conMoveYAxis);

    mouseButtons();
    getPosition();

    MoveXAxis = map (finalX, xLeft, xRight, 300, (res_x - 300));
    MoveYAxis = map (finalY, yTop, yBottom, 200, (res_y - 200));
    conMoveXAxis = constrain (MoveXAxis, 0, res_x);
    conMoveYAxis = constrain (MoveYAxis, 0, res_y);

    PrintResults();
    reset();

  }

}


/*        -----------------------------------------------        */
/* --------------------------- METHODS ------------------------- */
/*        -----------------------------------------------        */


void getPosition() {    // Get tilt adjusted position from IR postioning camera

  myDFRobotIRPosition.requestPosition();

  if (myDFRobotIRPosition.available()) {
    for (int i = 0; i < 4; i++) {
      positionX[i] = myDFRobotIRPosition.readX(i);
      positionY[i] = myDFRobotIRPosition.readY(i);
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
    
    finalX = 512 + cos(atan2(twoY - oneY, twoX - oneX) * -1) * (((oneX - twoX) / 2 + twoX) - 512) - sin(atan2(twoY - oneY, twoX - oneX) * -1) * (((oneY - twoY) / 2 + twoY) - 384);
    finalY = 384 + sin(atan2(twoY - oneY, twoX - oneX) * -1) * (((oneX - twoX) / 2 + twoX) - 512) + cos(atan2(twoY - oneY, twoX - oneX) * -1) * (((oneY - twoY) / 2 + twoY) - 384);
    
  }

  else {
    Serial.println("Device not available!");
  }

}



void go() {    // Setup Start Calibration Button

  buttonState1 = digitalRead(_reload);

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

  buttonState2 = digitalRead(_tigger);
  buttonState3 = digitalRead(_up);  
  buttonState4 = digitalRead(_down);
  buttonState5 = digitalRead(_left);
  buttonState6 = digitalRead(_right);   
  buttonState7 = digitalRead(_APin);
  buttonState8 = digitalRead(_BPin);
  buttonState9 = digitalRead(_startPin);      
  buttonState10 = digitalRead(_select); 

  
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
    Keyboard.press(KEY_UP_ARROW);
    delay(100);
    Keyboard.releaseAll();
    }
    else {

    }
    delay(10);
  }

  if (buttonState4 != lastButtonState4) {     
    if (buttonState4 == LOW) {
    Keyboard.press(KEY_DOWN_ARROW);
    delay(100);
    Keyboard.releaseAll();
    }
    else {

    }
    delay(10);
  }

    if (buttonState5 != lastButtonState5) {
    if (buttonState5 == LOW) {
    Keyboard.press(KEY_LEFT_ARROW);
    delay(100);
    Keyboard.releaseAll();
    }
    else {
 
    }
    delay(10);
  }
  
  if (buttonState6 != lastButtonState6) {
    if (buttonState6 == LOW) {
    Keyboard.press(KEY_RIGHT_ARROW);
    delay(100);
    Keyboard.releaseAll();
    }
    else {
 
    }
    delay(10);
  }

if (buttonState7 != lastButtonState7) {
    if (buttonState7 == LOW) {
      AbsMouse.press(MOUSE_RIGHT);
    }
    else {
      AbsMouse.release(MOUSE_RIGHT);
    }
    delay(10);
  }

  if (buttonState8 != lastButtonState8) {     
    if (buttonState8 == LOW) {
      AbsMouse.press(MOUSE_MIDDLE);
    }
    else {
      AbsMouse.release(MOUSE_MIDDLE);
    }
    delay(10);
  }
    if (buttonState9 != lastButtonState9) {
    if (buttonState9 == LOW) {
    Keyboard.press(KEY_BACKSPACE);
    delay(100);
    Keyboard.releaseAll();
    }
    else {
 
    }
    delay(10);
  }
  
  if (buttonState10 != lastButtonState10) {
    if (buttonState10 == LOW) {
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.releaseAll();
    }
    else {
 
    }
    delay(10);
  }

  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  lastButtonState4 = buttonState4;      
  lastButtonState5 = buttonState5;
  lastButtonState6 = buttonState6;
  lastButtonState7 = buttonState7;
  lastButtonState8 = buttonState8;
  lastButtonState9 = buttonState9;
  lastButtonState10 = buttonState10;      
}


void mouseCount() {    // Set count down on trigger

  buttonState2 = digitalRead(_tigger);

  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      count--;
    }
    else { // do nothing
    }
    delay(10);
  }

  lastButtonState2 = buttonState2;
}


void reset() {    // Pause/Re-calibrate button

  buttonState1 = digitalRead(_reload);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      count = 3;
      delay(50);
    }
    else { // do nothing
    }
    delay(50);
  }
  lastButtonState1 = buttonState1;
}


void skip() {    // Unpause button

  buttonState1 = digitalRead(_reload);

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


void PrintResults() {    // Print results for debugging

  Serial.print("RAW: ");
  Serial.print(finalX);
  Serial.print(", ");
  Serial.print(finalY);
  Serial.print("     Count: ");
  Serial.print(count);
  Serial.print("     Calibration: ");
  Serial.print(xLeft);
  Serial.print(", ");
  Serial.print(yTop);
  Serial.print(", ");
  Serial.print(xRight);
  Serial.print(", ");
  Serial.print(yBottom);
  Serial.print("     Position: ");
  Serial.print(conMoveXAxis);
  Serial.print(", ");
  Serial.println(conMoveYAxis);

}
