/***************************************************************
* Arduino library for Brushless Drive Wheels UGV
* by Rasheed Kittinanthapanya
* a communication is UART via Serial1 on Arduino MEGA board
***************************************************************/

#include <BrushlessWheels.h>

BrushlessWheels BW;

void setup() {

  Serial.begin(57600);

  // Do initialize 
  BW.Init();
  

}

void loop() {

  // Input two RPM values for both wheels
  BW.DriveWheels(60.0,60.0);
 

}
