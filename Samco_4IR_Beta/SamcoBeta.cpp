/*!
 * @file SamcoBeta.cpp
 * @brief Light Gun library for 4 LED setup
 * @n CPP file for Samco Light Gun 4 LED setup
 *
 * @copyright   Samco, https://github.com/samuelballantyne, May 2021
 * @copyright   GNU Lesser General Public License
 *
 * @author [Sam Ballantyne](samuelballantyne@hotmail.com)
 * @version  V1.0
 * @date  2021
 */

#include "SamcoBeta.h"
#include "Arduino.h"

void SamcoBeta::begin(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int cx, int cy) {

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
                  FinalX[0] = FinalX[2] + yDistLeft * cos(angleBottom + AngleOffset[2]);
                  FinalY[0] = FinalY[2] + yDistLeft * sin(angleBottom + AngleOffset[2]) * -1;

                } else if (positionX[i] < 0) {
                  FinalX[0] = FinalX[1] + xDistTop * cos(angleRight - AngleOffset[1]) * -1;
                  FinalY[0] = FinalY[1] + xDistTop * sin(angleRight - AngleOffset[1]);

                }
              } else if (positionX[i] > medianX) {
                if (see[1] > 1) {
                  FinalX[1] = positionX[i] - buff;
                  FinalY[1] = positionY[i] + buff;

                } else if (positionY[i] < 0) {
                  FinalX[1] = FinalX[3] + yDistRight * cos(angleBottom - (AngleOffset[3] - 3.14159));
                  FinalY[1] = FinalY[3] + yDistRight * sin(angleBottom - (AngleOffset[3] - 3.14159)) * -1;

                } else if (positionX[i] > 1023) {
                  FinalX[1] = FinalX[0] + xDistTop * cos(angleLeft + (AngleOffset[0] - 3.14159));
                  FinalY[1] = FinalY[0] + xDistTop * sin(angleLeft + (AngleOffset[0] - 3.14159)) * -1;
                  
                }
              }
              
             
              } else if (positionY[i] > medianY) {
              if (positionX[i] < medianX) {
                if (see[2] > 1) {
                  FinalX[2] = positionX[i] + buff;
                  FinalY[2] = positionY[i] - buff;

                } else if (positionY[i] > 768) {
                  FinalX[2] = FinalX[0] + yDistLeft * cos(angleTop - AngleOffset[0]);
                  FinalY[2] = FinalY[0] + yDistLeft * sin((angleTop - AngleOffset[0]) * -1);

                } else if (positionX[i] < 0) {
                  FinalX[2] = FinalX[3] + xDistBottom * cos(angleRight + AngleOffset[3]);
                  FinalY[2] = FinalY[3] + xDistBottom * sin(angleRight + AngleOffset[3])  * -1;

                }
              } else if (positionX[i] > medianX) {
                if ((see[3] > 1)) {
                  FinalX[3] = positionX[i] - buff;
                  FinalY[3] = positionY[i] - buff;

                } else if (positionY[i] > 768) {
                  FinalX[3] = FinalX[1] + yDistRight * cos(angleTop + (AngleOffset[1] - 3.14159));
                  FinalY[3] = FinalY[1] + yDistRight * sin(angleTop + (AngleOffset[1] - 3.14159)) * -1;

                } else if (positionX[i] > 1023) {
                  FinalX[3] = FinalX[2] + xDistBottom * cos(angleLeft - (AngleOffset[2] - 3.14159)) * -1;
                  FinalY[3] = FinalY[2] + xDistBottom * sin(angleLeft - (AngleOffset[2] - 3.14159));

                }
              }
            }
      }

// If all LEDS can be seen update median & angle offsets (resets sketch stop hangs on glitches)
        
      if ((positionYY[0] != 1023) && (positionYY[1] != 1023) && (positionYY[2] != 1023) && (positionYY[3] != 1023)){
        	medianY = (positionY[0] + positionY[1] + positionY[2] + positionY[3]) / 4;
        	medianX = (positionX[0] + positionX[1] + positionX[2] + positionX[3]) / 4;
      }
      else{
        medianY = (FinalY[0] + FinalY[1] + FinalY[2] + FinalY[3]) / 4;
        medianX = (FinalX[0] + FinalX[1] + FinalX[2] + FinalX[3]) / 4;
      }

// If 4 LEDS can be seen and loop has run through 5 times update offsets and height      

      if ((see[0] > 5) && (see[1] > 5) && (see[2] > 5) && (see[3] > 5)) {
	AngleOffset[0] = angleTop - (angleLeft - 3.14159);
    	AngleOffset[1] = (angleTop - angleRight) * -1;
    	AngleOffset[2] = (angleBottom - angleLeft) * -1;
    	AngleOffset[3] = angleBottom - (angleRight - 3.14159);
	height = ((yDistLeft + yDistRight) /2);
	}
      else {
	}

// If 2 LEDS can be seen and loop has run through 5 times update angle and distances

      if ((see[0] > 5) && (see[2] > 5)) {
    	angleLeft = atan2(FinalY[2] - FinalY[0], FinalX[0] - FinalX[2]);
    	yDistLeft = hypot((FinalY[0] - FinalY[2]), (FinalX[0] - FinalX[2]));
      } 
      else {
        yDistLeft = yDistLeft;
      }

      if ((see[3] > 5) && (see[1] > 5)) {
    	angleRight = atan2(FinalY[3] - FinalY[1], FinalX[1] - FinalX[3]);
    	yDistRight = hypot((FinalY[3] - FinalY[1]), (FinalX[3] - FinalX[1]));
      }
      else {
        yDistRight = yDistRight;
      }
      

      if ((see[0] > 5) && (see[1] > 5)) {
    	angleTop = atan2(FinalY[0] - FinalY[1], FinalX[1] - FinalX[0]);
    	xDistTop = hypot((FinalY[0] - FinalY[1]), (FinalX[0] - FinalX[1]));
      }  
      else {
        xDistTop = xDistTop;
      }

      if ((see[3] > 5) && (see[2] > 5)) {
    	angleBottom = atan2(FinalY[2] - FinalY[3], FinalX[3] - FinalX[2]);
    	xDistBottom = hypot((FinalY[2] - FinalY[3]), (FinalX[2] - FinalX[3]));
      }
      else {
        xDistBottom = xDistBottom;
      }

// Add tilt correction
      angle = (atan2(FinalY[0] - FinalY[1], FinalX[1] - FinalX[0]) + atan2(FinalY[2] - FinalY[3], FinalX[3] - FinalX[2])) / 2;
      xx = cx + cos(angle) * (medianX - cx) - sin(angle) * (medianY - cy);
      yy = cy + sin(angle) * (medianX - cx) + cos(angle) * (medianY - cy);
      
    }
    else{}
}


int SamcoBeta::testX(int index) // used of trouble shooting sketch
{
  return FinalX[index];
}

int SamcoBeta::testY(int index) // used of trouble shooting sketch
{
  return FinalY[index];
}

int SamcoBeta::testSee(int index) // used of trouble shooting sketch
{
  return see[index];
}

int SamcoBeta::testMedianX() // used of trouble shooting sketch
{
  return medianX;
}

int SamcoBeta::testMedianY() // used of trouble shooting sketch
{
  return medianY;
}

int SamcoBeta::X() // Final Values
{
  return xx;
}

int SamcoBeta::Y() // Final Values
{
  return yy;
}

int SamcoBeta::H() // Final Values
{
  return height;
}
    