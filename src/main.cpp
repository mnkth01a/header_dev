// motors.ino

#include "globals.h"
#include "A4988.h"
#include "i2c_stuff.h"

// using namespaces;
using namespace Globals;
using namespace A4988;
using namespace I2C_STUFF;

/*************************************************************/
/* Interrupt Service Routines.                               */
/*************************************************************/
/** ISR booleans **/
volatile bool answer;

void button_ISR()
{
  answer = digitalRead(ledPIN);
  digitalWrite(ledPIN, !answer);
  // tellProcessing();
  cycleCount++;
  previouscycleTime = millis();
}

/*************************************************************/
/* Arduino Main Program Routines.                            */
/*************************************************************/
/** Only runs once, you know. **/
void setup()
{
  Serial.begin(BAUD_RATE);

  whoIam();

  i2c_scan();

  // Set the digital output pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(MS1Pin, OUTPUT);
  pinMode(MS2Pin, OUTPUT);
  pinMode(MS3Pin, OUTPUT);
  pinMode(RESETPin, OUTPUT);
  pinMode(SLEEPPin, OUTPUT);
  pinMode(ENABLEPin, OUTPUT);
  pinMode(ledPIN, OUTPUT);

  digitalWrite(RESETPin, RESET);
  digitalWrite(SLEEPPin, SLEEP);
  digitalWrite(ENABLEPin, ENABLE);

  // Initialize hardware interrupt for the buttonPIN
  attachInterrupt(digitalPinToInterrupt(buttonPIN), button_ISR, FALLING);
}

/** Infinite Loop. Could've done that with a while(TRUE). **/
void loop()
{
  digitalWrite(ENABLEPin, !ENABLE);
  // Set cursor to the top left corner and print the string on the first row
  lcd.setCursor(15, 0);
  lcd.print("DISBL");

  Serial.println("Choose a function:");
  Serial.println("1: A4988_DEMO()");
  Serial.println("2: continuous_run(true = CW false = CCW, speed 10 - 100)");
  Serial.println();

  while (Serial.available() == 0)
  {
    // wait for user input
  }
  int a = Serial.readStringUntil('\n').toInt();

  digitalWrite(ENABLEPin, ENABLE);
  // Set cursor to the top left corner and print the string on the first row
  lcd.setCursor(15, 0);
  lcd.print("ENABL");

  delay(300);

  /** Here's the meat of the program.
  "Where's the beef?" It's right here! **/
  switch (a)
  {
  // A4988 Stepper Motor Demo fun function.
  case 1:
    A4988_DEMO();
    Serial.flush();
    lcd.clear();
    break;

  // A4988 Stepper Motor run, run, run...
  case 2:
    set_cont_run_params();
    Serial.println();
    Serial.println("Press Enter to stop");
    while (Serial.available() == 0)
    {
      continuous_run(udir, pulses, spd_dly);
    }
    Serial.readStringUntil('\n');
    Serial.flush();
    lcd.clear();
    break;

  default:
    // TODO: Let's put an exception handler here, what d'ya think?
    break;
  }
}
