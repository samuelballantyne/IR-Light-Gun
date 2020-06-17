/*!
 * @file Samco.h
 * @brief Samco Light Gun library for 4 LED setup
 * @n Header file for Samco Light Gun 4 LED setup
 *
 * @copyright  [Samco](http://www.samco.co.nz), 2020
 * @copyright GNU Lesser General Public License
 *
 * @author [Sam Ballantyne](samuelballantyne@hotmail.com)
 * @version  V1.0
 * @date  2020
 */

#include "Arduino.h"

#ifndef Samco_h
  #define Samco_h

class Samco {
  
private:

  int positionXX[4];   ///< position x.
  int positionYY[4];   ///< position y.

  int positionX[4]; 
  int positionY[4];

  int see[4];
  int buff = 50;

  int medianY = 768 / 2;
  int medianX = 1024 / 2;

  int FinalX[4] = {400,623,400,623};
  int FinalY[4] = {200,200,568,568};

  int xDistTop;
  int xDistBottom;
  int yDistLeft;
  int yDistRight;

  float angle;
  float angleOffset;

  int xx;
  int yy;

  int i;  
  
  int start = 0;

public:

void mapper(); 
void begin(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int cx, int cy);
int testX(int index);  
int testY(int index);
int testMedianX();
int testMedianY();
int X();
int Y();

 
};


#endif
