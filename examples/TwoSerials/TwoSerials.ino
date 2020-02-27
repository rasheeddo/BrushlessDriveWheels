#include <BrushlessWheels.h>
#include <SoftwareSerial.h>

//Software Serial (RX,TX)
SoftwareSerial wheelSerial(11,10);

BrushlessWheels BW(&wheelSerial);

void setup() {
  // for PC uart prinln
  Serial.begin(115200);

  // baudrate of wheel's esc must be 9600
  while(!(BW.serialBegin(9600)));
  // initialize the wheel's esc
  BW.Init();

  Serial.println("Initialized complete!");
}

void loop() {
  
  // Input two RPM values for both wheels
  BW.DriveWheels(60.0,60.0);

}
