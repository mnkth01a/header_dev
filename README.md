# A4988.h

   A4988 Step Motor Driver has a 1.8 degree step angle that relates to 360 degs / 1.8 degs per pulse = 200 pulses @ full step.

   200 pulses @:
      full step = 360 degs.         x 1 = 360 degs.
      half step = 180 degs.         x 2 = 360 degs.
      quarter step = 90 degs.       x 4 = 360 degs.
      eighth step = 45 degs.        x 8 = 360 degs.
      sixteenth step = 22.5 degs.   x 16 = 360 degs.

   The way the pulses variable works is:
      turn the pulse on,
      delay from approx 400 to 2000 microseconds,
      then turn the pulse off,
      delay again for the same amount of time.

   Higher numbers = slower speed.
   Lower numbers = faster speed.

   According to the datasheets for the A4988 and the NEMA 17, the degrees per step is:

      Step Mode:                  Motor:            Driver:
      
      Full step:                 1.8 degs          90 degs
      Half step:                 0.9 degs          45 degs
      Quarter step:              0.45 degs         22.5 degs
      Eighth step:               0.225 degs        11.25 degs
      Sixteenth step:            0.1125 degs       5.625 degs

   I have no idea what the difference is, but I'm using the motor values.  It seems to work fairly accurately

   The A4988 has a 1/16 microstepping feature.  This means that the motor can be moved in 1/16 increments of a step.  This is done by setting the MS1, MS2, and MS3 pins to HIGH or LOW.  The table below shows the microstepping resolution for the A4988.

      MS1 MS2 MS3      Microstep Resolution
         Low Low Low        Full step
         High Low Low       Half step
         Low High Low       Quarter step
         High High Low      Eighth step
         High High High     Sixteenth step

   The A4988 has a RESET, SLEEP, and ENABLE pin.  The RESET pin sets the translator to a predefined Home state and turns off all of the FET outputs.  All STEP inputs are ignored until the RESET input is set to high.  The SLEEP pin turns off the FET outputs, current regulator, and charge pump for maximum power savings.  Allow 1 ms after Sleep mode, in order to allow the charge pump to stabilize, before issuing any STEP pulses.  When set to logic high, the outputs are disabled.  The translator inputs STEP, DIR, and MSx, as well as the internal sequencing logic, all remain active, independent of the ENABLE input state.

   The A4988 has a DIR pin that sets the direction of rotation.  The DIR pin is low for counterclockwise rotation and high for clockwise rotation, if properly wired. (I know, it's not that big of a deal, but it's a pet peeve of mine.  I like consistency, so positive direction goes clockwise.  However, mathematically, positive direction is counterclockwise due to the numbering of the quadrants.  It's easy enough to change.)

   The goals for this header file are (in no particular order):

      1.  Break functions into atomic parts.  
      This will allow the programmer to use the functions in different ways.


      2.  Use millis to make the Arduino pseudo multitask.


      3.  Use interrupts to make the Arduino pseudo multitask.


      4.  Use a state machine to make the Arduino pseudo multitask.


      5.  Develop A4988 class in the namespace A4988.


      6.  Need to implement exception handling.


      7.  Set up working with radians. (This will require a refresher in trigonometry.)


      8.  Keep documentation updated.  Add datasheets to the documentation.
