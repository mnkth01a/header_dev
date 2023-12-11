#pragma once
#ifndef LCDI2C2004_H
#define LCDI2C2004_H

#include <Arduino.h>

/***************************************************************************
*name : I2C LCD2004
***************************************************************************/

// Email:service@sunfounder.com
// Website:www.sunfounder.com

/********************************/
// Include necessary libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD object, set the LCD I2C address to 0x27 for a 20x4 display
LiquidCrystal_I2C lcd(0x27, 20, 4);

/*********************************************************/

class LCDI2C2004 : public LiquidCrystal_I2C
{

public:
   struct d2004
   {
      String row1; // cursor position 0,0
      String row2; // cursor position 0,1
      String row3; // cursor position 0,2
      String row4; // cursor position 0,3
   };

private:
};

#endif // LCDI2C2004_H