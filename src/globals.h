#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*********************************************************/

namespace Globals
{
   /** Constants **/
   const int DEVICE_ID = 0x0a; // Change this to the ID of this device
   const char DEVICE_NAME[] = "Motors";
   const int BAUD_RATE = 9600;

   // Initialize the LCD object, set the LCD I2C address to 0x27 for a 20x4 display
   LiquidCrystal_I2C lcd(0x27, 20, 4);

   const int ledPIN = 13; // LED1 connected to digital pin 13
   const int buttonPIN = 1;

   bool ledState;
   bool lastButtonState;
   bool buttonState;

   byte cycleCount = 0;

   unsigned long currentTime = 0;
   unsigned long previousblinkTime = 0;
   unsigned long previousfadeTime = 0;
   unsigned long previouscycleTime = 0;

   void whoIam()
   {
      // Display the microcontroller's information to the serial monitor
      Serial.println("Device Name: " + String(DEVICE_NAME));
      Serial.println("CPU: " + String(__AVR_ARCH__));
      Serial.println("Clock Speed: " + String(F_CPU));
      Serial.println("ARDUINO: " + String(ARDUINO));
      Serial.println("ARDUINO_AVR_UNO: " + String(ARDUINO_AVR_UNO));
      Serial.println();
      Serial.flush();
   }

   bool button_debounce(boolean last)
   {
      boolean current = digitalRead(buttonPIN);
      if (last != current)
      {
         delay(30);
         current = digitalRead(buttonPIN);
      }
      return current;
   }

   void tellProcessing()
   {
      // check if button is pressed and if so, send a message to the serial monitor
      buttonState = button_debounce(lastButtonState);

      if (buttonState == LOW && lastButtonState == HIGH)
      {
         for (int i = 0; i < 3; i++)
         {
            Serial.println("Pink");
         }
      }

      if (buttonState == HIGH && lastButtonState == LOW)
      {
         for (int i = 0; i < 3; i++)
         {
            Serial.println("Purple");
         }
      }
      Serial.flush();

      lastButtonState = buttonState;
   }

   void blink()
   {
      currentTime = millis();
      if (currentTime - previousblinkTime >= 1000)
      {
         previousblinkTime = currentTime;
         ledState = !ledState;
         digitalWrite(ledPIN, ledState);
      }
   }

}

#endif // GLOBALS_H
