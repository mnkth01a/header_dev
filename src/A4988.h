#pragma once
#ifndef A4988_H
#define A4988_H

#include "globals.h"

using namespace Globals;

/***************************************************************************/
namespace A4988
{
   // defines pins numbers
   const int stepPin = 3;
   const int dirPin = 4;

   //
   const int MS1Pin = 5;
   const int MS2Pin = 6;
   const int MS3Pin = 7;

   //
   const int RESETPin = 8;
   const int SLEEPPin = 9;
   const int ENABLEPin = 10;

   const int pulses_x1 = 200; // 360 degrees / 1.8 degrees.
   const int pulses_x2 = 400; // 720 degrees / 1.8 degrees.

   int pulses = 0;
   int pulses2 = 0;

   int unit = 0;

   double pulsesPerDegree = 0.55555556;
   double pulsesPerRadian = 0.0308641975;

   double degreesPerPulse = 1.8;
   double radiansPerPulse = 0.0314159265;

   double degrees = 0;
   double degrees2 = 0;

   double rads = 0;
   double rads2 = 0;

   double degreesPerRadian = 57.2958;
   double radiansPerDegree = 0.0174533;

   // Motor direction bit
   // true = clockwise,
   // false = counterclockwise
   bool dir = true;

   // Direction of rotation variables for the driver.
   int direction = 1;
   int udir = 1;

   // Number of rounds to rotate the motor a full circuit around.
   int rounds = 0;

   /*      A4988 Driver Modes
    * MS1 MS2 MS3      Microstep Resolution
    * Low Low Low          Full step
    * High Low Low         Half step
    * Low High Low         Quarter step
    * High High Low        Eighth step
    * High High High       Sixteenth step
    */
   bool MS1;
   bool MS2;
   bool MS3;

   // RESET input sets the translator to a predefined Home state and turns off all of the FET outputs.  All STEP inputs are ignored until the RESET input is set to high.
   bool RESET = true;

   // SLEEP input turns off the FET outputs, current regulator, and charge pump for maximum power savings.  Allow 1 ms after Sleep mode, in order to allow the charge pump to stabilize, before issuing any STEP pulses.
   bool SLEEP = true;

   // When set to logic high, the outputs are disabled.  The translator inputs STEP, DIR, and MSx, as well as the internal sequencing logic, all remain active, independent of the ENABLE input state.
   bool ENABLE = false;

   // The static integer spd_dly is used to set the speed of the motor.  The speed is set by the user in the set_run_params() function.
   static int spd_dly = 0;

   int minSpeed = 2000;
   int maxSpeed = 400;

   int minStepSpeed = 0;
   int maxStepSpeed = 0;

   // switch gear.
   void shift_gears(int gear)
   {
      // Disable the enable. This ca be changed to RESET instead to effectively brake the motor.
      digitalWrite(ENABLEPin, !ENABLE);

      // Set cursor to the top left corner and print the string on the first row
      lcd.setCursor(15, 0);
      lcd.print("DISBL");

      switch (gear)
      {
      case 1: // full step
         minStepSpeed = 1200;
         maxStepSpeed = 400;
         MS1 = false;
         MS2 = false;
         MS3 = false;
         break;

      case 2: // half step
         minStepSpeed = 1200;
         maxStepSpeed = 400;
         MS1 = true;
         MS2 = false;
         MS3 = false;
         break;

      case 3: // quarter step
         minStepSpeed = 1400;
         maxStepSpeed = 400;
         MS1 = false;
         MS2 = true;
         MS3 = false;
         break;

      case 4: // eighth step
         minStepSpeed = 1400;
         maxStepSpeed = 400;
         MS1 = true;
         MS2 = true;
         MS3 = false;
         break;

      case 5: // sixteenth step
         minStepSpeed = 1600;
         maxStepSpeed = 400;
         MS1 = true;
         MS2 = true;
         MS3 = true;
         break;

      default:
         // TODO: Add an exception handler here, please sir.
         break;
      }
      // set the step mode.  (saythatfivetimesfast)
      digitalWrite(MS1Pin, MS1);
      digitalWrite(MS2Pin, MS2);
      digitalWrite(MS3Pin, MS3);

      // set the speed range for the current step mode.
      minSpeed = minStepSpeed;
      maxSpeed = maxStepSpeed;

      // Reenable the enable. This can be changed to RESET instead to effectively brake the motor.
      digitalWrite(ENABLEPin, ENABLE);

      // Set cursor to the top left corner and print the string on the first row
      lcd.setCursor(15, 0);
      lcd.print("ENABL");

      delay(300);
   } // shift_gears

   /*
         Set the unit to use.  Degrees or Radians
      */
   int getUnits()
   {
      Serial.println("Choose units: 1 = degrees, 2 = radians.");
      while (Serial.available() == 0)
      {
         // wait for user input
      }
      int units = Serial.readStringUntil('\n').toInt();

      switch (units)
      {
      case 1:
         Serial.println("Degrees.");
         Serial.println("Enter degrees to rotate the motor clockwise:");
         while (Serial.available() == 0)
         {
            // wait for user input
         }
         degrees = Serial.readStringUntil('\n').toDouble();

         if (degrees < 0)
         {
            degrees = -degrees;
         }
         else if (degrees == 0.00)
         {
            degrees = 45.00;
         }
         Serial.println("Degrees = " + String(degrees) + " degrees.");

         if (degrees < 180)
         {
            degrees2 = 360 + (2 * degrees);
         }
         else if (degrees == 180)
         {
            degrees2 = 540;
         }
         else if ((degrees > 180) && (degrees < 360))
         {
            degrees2 = 360 - degrees;
         }
         else if (degrees > 360)
         {
            degrees2 = degrees - 360;
         }
         else if (degrees == 360)
         {
            degrees2 = 405;
         }
         Serial.println("Degrees2 = " + String(degrees2) + " degrees.");
         rads = int(degrees * radiansPerDegree);
         break;

         // TODO: This ain't working.  Fix it.
      case 2:
         Serial.println("Radians.");
         Serial.println("Enter radians to rotate the motor clockwise:");
         while (Serial.available() == 0)
         {
            // wait for user input
         }
         rads = Serial.readStringUntil('\n').toDouble();

         if (rads < 0)
         {
            rads = -rads;
         }
         else if (rads == 0.00)
         {
            rads = 45.00;
         }
         Serial.println("Radians = " + String(rads) + " rads.");

         if (rads < 180)
         {
            rads2 = 360 + (2 * rads);
         }
         else if (rads == 180)
         {
            rads2 = 540;
         }
         else if ((rads > 180) && (rads < 360))
         {
            rads2 = 360 - rads;
         }
         else if (rads > 360)
         {
            rads2 = rads - 360;
         }
         else if (rads == 360)
         {
            rads2 = 405;
         }

         Serial.println("Radians2 = " + String(rads2) + " rads.");
         degrees = rads * degreesPerRadian;
         break;

      default:
         // TODO: Add an exception handler here, young'un.
         Serial.println("Degrees = 45 and Degrees2 = 450, by default.");
         Serial.println();

         degrees = 45;
         degrees2 = 450;

         rads = int(degrees * radiansPerDegree);
         break;
      }
      return units;
   } // getUnits

   // sets the number of pulses to rotate the motor.
   // Can be degrees or radians. (at some point)
   void set_pulses(int stepMode)
   {
      // TODO: Check degrees and radians for a valid value.

      switch (stepMode)
      {
         // TODO: Add a case for radians. How to decide between degrees and radians?  Or, just use degrees?  Convert radians to degrees?
      case 1:
         pulses = int(degrees * pulsesPerDegree);
         pulses2 = int(degrees2 * pulsesPerDegree);
         break;

      case 2:
         pulses = int(degrees * pulsesPerDegree) * 2;
         pulses2 = int(degrees2 * pulsesPerDegree) * 2;
         break;

      case 3:
         pulses = int(degrees * pulsesPerDegree) * 4;
         pulses2 = int(degrees2 * pulsesPerDegree) * 4;
         break;

      case 4:
         pulses = int(degrees * pulsesPerDegree) * 8;
         pulses2 = int(degrees2 * pulsesPerDegree) * 8;
         break;

      case 5:
         pulses = int(degrees * pulsesPerDegree) * 16;
         pulses2 = int(degrees2 * pulsesPerDegree) * 16;
         break;

      default:
         // TODO: Looks like a good place for an exception handler!
         Serial.println("Step Mode is not valid in set_pulses.");
         break;
      }
   } // set_pulses

   /*
         Steps the motor
      */
   void step_motor(int pulses, int speed_delay)
   {
      if ((pulses != 0) and (speed_delay > 0))
      {
         for (int x = 0; x < pulses; x++)
         {
            noInterrupts();
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(speed_delay);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(speed_delay);
            interrupts();
         }
      }
   } // step_motor

   int setRounds(int degs)
   {
      if (degs < 0)
      {
         degs = -degs;
      }
      else if (degs == 0)
      {
         degs = 360;
      }

      // if degs is less than or equal to 180, then rounds = 360 / degs.
      if (degs <= 180)
      {
         rounds = round(360 / degs);
      }

      // else if degs is between 180 and 360, then rounds = 360 / (360 - degs).
      else if ((degs > 180) && (degs < 360))
      {
         rounds = round(360 / (360 - degs));
      }

      // else if degs is greater than 360, then rounds = 360 / (degs - 360).
      else if (degs > 360)
      {
         rounds = round(360 / (degs - 360));
      }

      // else if degs is equal to 360, then rounds = 360 / (405 - degs).
      else if (degs == 360)
      {
         rounds = round(360 / (405 - degs));
      }

      Serial.println("Rounds = " + String(rounds));

      return rounds;
   } // setRounds

   /* This demo does some fun stuff */
   void A4988_DEMO()
   {
      /*********************************************************/
      /* Initialize the LCD display                            */
      /*********************************************************/

      lcd.init();      // Initialize the LCD
      lcd.backlight(); // Turn on the backlight
      lcd.clear();     // Clear the screen

      /*********************************************************/

      Serial.println("A4988 Step Motor Driver DEMO");
      Serial.println();

      // TODO: "Houston, we have a problem."  When set at 540 degrees.
      // Done.  Forgot to handle degrees2 when degrees is greater than or equal to 360 degrees.

      /*
         get units sets the degrees or radians, and sets the pulses value
         based on the user's input. It returns the units value from the User.
      */
      unit = getUnits();

      rounds = setRounds(degrees);

      // Step through the five step modes
      for (int k = 1; k < 6; k++)
      {
         set_pulses(k);
         shift_gears(k);

         // Step through the speed range
         for (int i = maxSpeed; i < minSpeed + 1; i += 200)
         {
            Serial.print("Step Mode = " + String(k) + ": ");

            // Set cursor to the top left corner and print the string on the first row
            lcd.setCursor(0, 0);
            lcd.print("Step   =");

            lcd.setCursor(9, 0);
            lcd.print(String(k));

            spd_dly = i;
            switch (k)
            {
            case 1:
               Serial.println(" full step aka 5th gear");
               break;

            case 2:
               Serial.println(" half step aka 4th gear");
               break;

            case 3:
               Serial.println(" quarter step aka 3rd gear");
               break;

            case 4:
               Serial.println(" eighth step aka 2nd gear");
               break;

            case 5:
               Serial.println(" sixteenth step aka 1st gear");
               break;

            default:
               // TODO: Add an exception handler here, please sir.
               break;
            }
            Serial.println("Speed = " + String(spd_dly) + " microseconds delay");
            // Move to the second row and print the string
            lcd.setCursor(0, 1);
            lcd.print("Speed  =");
            lcd.print("  ");

            lcd.setCursor(9, 1);
            lcd.print(String(spd_dly) + " ");

            lcd.setCursor(14, 1);
            lcd.print("msecs.");

            // Step through the number of rounds for 360 degrees
            for (int r = 1; r < rounds + 1; r++)
            {
               Serial.println("Rounds = " + String(rounds - r + 1));

               // Move to the third row and print the string
               lcd.setCursor(0, 2);
               lcd.print("Rounds =");

               lcd.setCursor(9, 2);
               lcd.print(String(rounds - r + 1) + " ");

               lcd.setCursor(14, 2);
               lcd.print("of " + String(rounds) + ".");

               dir = true; // Enables the motor to move in a clockwise direction
               digitalWrite(dirPin, dir);
               step_motor(pulses, spd_dly);

               // Move to the fourth row and print the string
               lcd.setCursor(0, 3);
               lcd.print("Degs   =");
               lcd.print("  ");

               lcd.setCursor(9, 3);
               lcd.print(String(int(degrees)) + "  ");

               lcd.setCursor(14, 3);
               lcd.print("degs.");

               delay(1000);

               dir = false; // Changes the rotation direction to counterclockwise
               digitalWrite(dirPin, dir);
               step_motor(pulses2, spd_dly);

               // Move to the fourth row and print the string
               lcd.setCursor(0, 3);
               lcd.print("Degs2  =");
               lcd.print("  ");

               lcd.setCursor(9, 3);
               lcd.print(String(int(degrees2))+ "  ");

               lcd.setCursor(14, 3);
               lcd.print("degs.");

               delay(1000);
            }
         }
         Serial.println();
      }
   } // A4988_DEMO

   void set_cont_run_params()
   {
      /*********************************************************/
      /* Initialize the LCD display                            */
      /*********************************************************/

      lcd.init();      // Initialize the LCD
      lcd.backlight(); // Turn on the backlight

      /*********************************************************/

      Serial.println("A4988 Step Motor Driver DEMO");
      Serial.println();

      Serial.println("Enter speed (1 - 100):");
      while (Serial.available() == 0)
      {
         // wait for user input
      }
      int speed = Serial.readStringUntil('\n').toInt();
      speed = constrain(speed, 1, 100);

      degrees = 360;

      Serial.println("Speed = " + String(speed) + " %");

      if ((speed >= 1) && (speed <= 19))
      {
         Serial.println("Shift to 1st gear.");
         Serial.println();
         shift_gears(5);
         spd_dly = map(speed, 1, 19, minStepSpeed, maxStepSpeed);
         pulses = int(degrees * pulsesPerDegree) * 16;
      }

      if ((speed >= 20) && (speed <= 39))
      {
         Serial.println("Shift to 2nd gear.");
         Serial.println();
         shift_gears(4);
         spd_dly = map(speed, 20, 39, minStepSpeed, maxStepSpeed);
         pulses = int(degrees * pulsesPerDegree) * 8;
      }

      if ((speed >= 40) && (speed <= 59))
      {
         Serial.println("Shift to 3rd gear.");
         Serial.println();
         shift_gears(3);
         spd_dly = map(speed, 40, 59, minStepSpeed, maxStepSpeed);
         pulses = int(degrees * pulsesPerDegree) * 4;
      }

      if ((speed >= 60) && (speed <= 79))
      {
         Serial.println("Shift to 4th gear.");
         Serial.println();
         shift_gears(2);
         spd_dly = map(speed, 60, 79, minStepSpeed, maxStepSpeed);
         pulses = int(degrees * pulsesPerDegree) * 2;
      }

      if ((speed >= 80) && (speed <= 100))
      {
         Serial.println("Shift to 5th gear.");
         Serial.println();
         shift_gears(1);
         spd_dly = map(speed, 80, 100, minStepSpeed, maxStepSpeed);
         pulses = int(degrees * pulsesPerDegree);
      }

      Serial.println("Spd_dly = " + String(spd_dly) + " microseconds delay.");
      Serial.println("Pulses = " + String(pulses) + " pulses.");
      Serial.println();

      Serial.println("Enter direction (1 = CW, 0 = CCW):");
      while (Serial.available() == 0)
      {
         // wait for user input
      }
      direction = Serial.readStringUntil('\n').toInt();
      udir = constrain(direction, 0, 1);
      // Serial.flush();

      lcd.setCursor(0, 0);
      lcd.print("Speed = " + String(speed) + " %");
      lcd.setCursor(0, 1);
      if (udir == 1)
         lcd.print("Dir   = CW");
      else
         lcd.print("Dir   = CCW");
      lcd.setCursor(0, 2);
      lcd.print("Spd_dly  = " + String(spd_dly));
      lcd.setCursor(0, 3);
      lcd.print("Pulses   = " + String(pulses));
   } // set_cont_run_params

   void continuous_run(bool dir, int pulses, int speed_delay)
   {
      digitalWrite(dirPin, dir);
      step_motor(pulses, speed_delay);
   } // continuous_run
} // namespace A4988

#endif // A4988_H
