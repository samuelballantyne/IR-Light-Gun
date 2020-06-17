/*!
 * @file Samco.cpp
 * @brief Light Gun library for 4 LED setup
 * @n CPP file for Samco Light Gun 4 LED setup
 *
 * @copyright   Samco, https://github.com/samuelballantyne, April 2020
 * @copyright   GNU Lesser General Public License
 *
 * @author [Sam Ballantyne](samuelballantyne@hotmail.com)
 * @version  V1.0
 * @date  2020
 */

#include "Samco.h"
#include "Arduino.h"

void Samco::begin(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int cx, int cy) {

// Remapping LED postions to use with library.
  
positionXX[0] = x0;
positionYY[0] = y0;
positionXX[1] = x1;
positionYY[1] = y1;
positionXX[2] = x2;
positionYY[2] = y2;
positionXX[3] = x3;
positionYY[3] = y3;

// Wait for all postions to be recognised before starting

    if ((positionYY[0] != 1023) && (positionYY[1] != 1023) && (positionYY[2] != 1023) && (positionYY[3] != 1023)) {
      start = 1;
    }

    if ((start = 1) && ((positionYY[0] != 1023) || (positionYY[1] != 1023) || (positionYY[2] != 1023) || (positionYY[3] != 1023))) {

      for (int i = 0; i < 4; i++) {
        
// Check if LED has been seen...

        if ( (positionYY[i] == 1023) && (positionXX[i] == 1023) ) {

// if unseen make sure all quadrants have a value if missing apply value with buffer and set to unseen (this step is important for 1 LED usage)

          if (!(((positionY[0] < medianY) && (positionX[0] < medianX)) || ((positionY[1] < medianY) && (positionX[1] < medianX)) || ((positionY[2] < medianY) && (positionX[2] < medianX)) || ((positionY[3] < medianY) && (positionX[3] < medianX)))) {
              positionX[i] = medianX + (medianX - FinalX[3]) - buff;
              positionY[i] = medianY + (medianY - FinalY[3]) - buff;
              see[0] = 0;
            }
          if (!(((positionY[0] < medianY) && (positionX[0] > medianX)) || ((positionY[1] < medianY) && (positionX[1] > medianX)) || ((positionY[2] < medianY) && (positionX[2] > medianX)) || ((positionY[3] < medianY) && (positionX[3] > medianX)))) {
              positionX[i] = medianX + (medianX - FinalX[2]) + buff;
              positionY[i] = medianY + (medianY - FinalY[2]) - buff;
              see[1] = 0;
            }
          if (!(((positionY[0] > medianY) && (positionX[0] < medianX)) || ((positionY[1] > medianY) && (positionX[1] < medianX)) || ((positionY[2] > medianY) && (positionX[2] < medianX)) || ((positionY[3] > medianY) && (positionX[3] < medianX)))) {
              positionX[i] = medianX + (medianX - FinalX[1]) - buff;
              positionY[i] = medianY + (medianY - FinalY[1]) + buff;
              see[2] = 0;
            }
          if (!(((positionY[0] > medianY) && (positionX[0] > medianX)) || ((positionY[1] > medianY) && (positionX[1] > medianX)) || ((positionY[2] > medianY) && (positionX[2] > medianX)) || ((positionY[3] > medianY) && (positionX[3] > medianX)))) {
              positionX[i] = medianX + (medianX - FinalX[0]) + buff;
              positionY[i] = medianY + (medianY - FinalY[0]) + buff;
              see[3] = 0;
            }

// if all quadrants have a value apply value with buffer and set to see/unseen
          
          if (positionY[i] < medianY){
              if (positionX[i] < medianX){
                positionX[i] = medianX + (medianX - FinalX[3]) - buff;
                positionY[i] = medianY + (medianY - FinalY[3]) - buff;
                see[0] = 0;
              }
          }
          if (positionY[i] < medianY){
              if (positionX[i] > medianX){
                positionX[i] = medianX + (medianX - FinalX[2]) + buff;
                positionY[i] = medianY + (medianY - FinalY[2]) - buff;
                see[1] = 0;
              }
          }           
          if (positionY[i] > medianY){
              if (positionX[i] < medianX){
                positionX[i] = medianX + (medianX - FinalX[1]) - buff;
                positionY[i] = medianY + (medianY - FinalY[1]) + buff;
                see[2] = 0;
              }
          }
          if (positionY[i] > medianY){
              if (positionX[i] > medianX){
                positionX[i] = medianX + (medianX - FinalX[0]) + buff;
                positionY[i] = medianY + (medianY - FinalY[0]) + buff;
                see[3] = 0;
              }
            }

// If LEDS have been seen place in correct quadrant, apply buffer an set to seen.

        } else {

          if (positionYY[i] < medianY){
              if (map(positionXX[i], 0, 1023, 1023, 0) < medianX){
                positionX[i] = map(positionXX[i], 0, 1023, 1023, 0) - buff;
                positionY[i] = positionYY[i] - buff;
                see[0] = see[0] + 1;
              }
              else if (map(positionXX[i], 0, 1023, 1023, 0) > medianX){
                positionX[i] = map(positionXX[i], 0, 1023, 1023, 0) + buff;
                positionY[i] = positionYY[i] - buff;
                see[1] = see[1] + 1;
              }
            }           
            else if (positionYY[i] > medianY){
              if (map(positionXX[i], 0, 1023, 1023, 0) < medianX){
                positionX[i] = map(positionXX[i], 0, 1023, 1023, 0) - buff;
                positionY[i] = positionYY[i] + buff;
                see[2] = see[2] + 1;
              }
              else if (map(positionXX[i], 0, 1023, 1023, 0) > medianX){
                positionX[i] = map(positionXX[i], 0, 1023, 1023, 0) + buff;
                positionY[i] = positionYY[i] + buff;
                see[3] = see[3] + 1;
              }
            }
          }

// Arrange all values in to quadrants and remove buffer.
// If LEDS have been seen use there value
// If LEDS haven't been seen work out values form live positions
      
          if (positionY[i] < medianY) {
              if (positionX[i] < medianX) {
                if (see[0] > 1) { 
                  FinalX[0] = positionX[i] + buff;
                  FinalY[0] = positionY[i] + buff;

                } else if (positionY[i] < 0) { 
                  FinalX[0] = FinalX[2] + (yDistLeft * sin(angle));
                  FinalY[0] = FinalY[2] - (yDistLeft * cos(angle));

                } else if (positionX[i] < 0) {
                  FinalX[0] = FinalX[1] - (xDistTop * cos(angle));
                  FinalY[0] = FinalY[1] - (xDistTop * sin(angle));

                }
              } else if (positionX[i] > medianX) {
                if (see[1] > 1) {
                  FinalX[1] = positionX[i] - buff;
                  FinalY[1] = positionY[i] + buff;

                } else if (positionY[i] < 0) {
                  FinalX[1] = FinalX[3] + (yDistRight * sin(angle));
                  FinalY[1] = FinalY[3] - (yDistRight * cos(angle));

                } else if (positionX[i] > 1023) {
                  FinalX[1] = FinalX[0] + (xDistTop * cos(angle));
                  FinalY[1] = FinalY[0] + (xDistTop * sin(angle));
                  
                }
              }
              
             
              } else if (positionY[i] > medianY) {
              if (positionX[i] < medianX) {
                if (see[2] > 1) {
                  FinalX[2] = positionX[i] + buff;
                  FinalY[2] = positionY[i] - buff;

                } else if (positionY[i] > 768) {
                  FinalX[2] = FinalX[0] - (yDistLeft * sin(angle));
                  FinalY[2] = FinalY[0] + (yDistLeft * cos(angle));

                } else if (positionX[i] < 0) {
                  FinalX[2] = FinalX[3] - (xDistBottom * cos(angle));
                  FinalY[2] = FinalY[3] - (xDistBottom * sin(angle));

                }
              } else if (positionX[i] > medianX) {
                if ((see[3] > 1)) {
                  FinalX[3] = positionX[i] - buff;
                  FinalY[3] = positionY[i] - buff;

                } else if (positionY[i] > 768) {
                  FinalX[3] = FinalX[1] - (yDistRight * sin(angle));
                  FinalY[3] = FinalY[1] + (yDistRight * cos(angle));

                } else if (positionX[i] > 1023) {
                  FinalX[3] = FinalX[2] + (xDistBottom * cos(angle));
                  FinalY[3] = FinalY[2] + (xDistBottom * sin(angle));

                }
              }
            }
      }

// If all LEDS can be seen update median (resets sketch stop hangs on glitches)
        
      if ((positionYY[0] != 1023) && (positionYY[1] != 1023) && (positionYY[2] != 1023) && (positionYY[3] != 1023)){
        medianY = (positionY[0] + positionY[1] + positionY[2] + positionY[3]) / 4;
        medianX = (positionX[0] + positionX[1] + positionX[2] + positionX[3]) / 4;
        if (yDistLeft > yDistRight){
          angleOffset = asin((float)(yDistLeft-yDistRight)/(xDistTop)) / 2;
        }
        else if (yDistRight > yDistLeft){
          angleOffset = (asin((float)(yDistRight-yDistLeft)/(xDistTop)) * -1) / 2;
        }
        else{
          angleOffset = 0;
        }
      }
      else{
        medianY = (FinalY[0] + FinalY[1] + FinalY[2] + FinalY[3]) / 4;
        medianX = (FinalX[0] + FinalX[1] + FinalX[2] + FinalX[3]) / 4;
      }

// If 2 LEDS can be seen and loop has run through 5 times update angle and distances

      if ((see[0] > 5) && (see[2] > 5)) {
        angle = (atan2(FinalX[2] - FinalX[0], FinalY[2] - FinalY[0]) * -1);
        yDistLeft = hypot((FinalY[0] - FinalY[2]), (FinalX[0] - FinalX[2]));
      } 
      else {
        yDistLeft = yDistLeft;
      }

      if ((see[3] > 5) && (see[1] > 5)) {
        angle = (atan2(FinalX[3] - FinalX[1], FinalY[3] - FinalY[1]) * -1);
        yDistRight = hypot((FinalY[1] - FinalY[3]), (FinalX[1] - FinalX[3]));
      }
      else {
        yDistRight = yDistRight;
      }
      

      if ((see[0] > 5) && (see[1] > 5)) {
        angle = (atan2(FinalX[1] - FinalX[0], FinalY[1] - FinalY[0]) * -1) + 1.57 - angleOffset;
        xDistTop = hypot((FinalY[0] - FinalY[1]), (FinalX[0] - FinalX[1]));
      }  
      else {
        xDistTop = xDistTop;
      }

      if ((see[3] > 5) && (see[2] > 5)) {
        angle = (atan2(FinalX[3] - FinalX[2], FinalY[3] - FinalY[2]) * -1) + 1.57 + angleOffset;
        xDistBottom = hypot((FinalY[3] - FinalY[2]), (FinalX[3] - FinalX[2]));
      }
      else {
        xDistBottom = xDistBottom;
      }

// Add tilt correction
      
      xx = cx + cos(angle * -1) * (medianX - cx) - sin(angle * -1) * (medianY - cy);
      yy = cy + sin(angle * -1) * (medianX - cx) + cos(angle * -1) * (medianY - cy);
      
    }
    else{}
}


int Samco::testX(int index) // used of trouble shooting sketch
{
  return FinalX[index];
}

int Samco::testY(int index) // used of trouble shooting sketch
{
  return FinalY[index];
}

int Samco::testMedianX() // used of trouble shooting sketch
{
  return medianX;
}

int Samco::testMedianY() // used of trouble shooting sketch
{
  return medianY;
}

int Samco::X() // Final Values
{
  return xx;
}

int Samco::Y() // Final Values
{
  return yy;
}
    