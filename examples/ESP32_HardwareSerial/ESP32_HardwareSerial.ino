#include <BrushlessWheels.h>
//#include <HardwareSerial.h>

#define RXD1 12 // GPIO12
#define TXD1 13 // GPIO13

BrushlessWheels BW(&Serial1);

void setup() {

  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1);
  // baudrate of wheel's esc must be 9600
  //while(!(BW.serialBegin(9600)));
  // initialize the wheel's esc
  BW.Init();

}

void loop() {
  // put your main code here, to run repeatedly:

//  DriveWheels(60.0, 60.0);
  BW.DriveWheels(60.0, 60.0);

}
