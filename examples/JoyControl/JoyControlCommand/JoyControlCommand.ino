#include <BrushlessWheels.h>
#include <SoftwareSerial.h>
float a0;
float a1;
float rpmL;
float rpmR;
float RPM[2];

float scale_a0;

float MaxRPM = 144.0;
float ThirdMaxRPM = MaxRPM/3.0;
float HalfMaxRPM = MaxRPM/2.0;
float QuatMaxRPM = MaxRPM/4.0;

SoftwareSerial wheelSerial(11,10);
BrushlessWheels BW(&wheelSerial);


void GetJoyCommand(float Output[2])
{
  a0 = analogRead(0);
  a1 = analogRead(1);

  //Serial.print("a0 ");
  //Serial.println(a0);
  //Serial.print("a1 ");
  //Serial.println(a1);
  
  // push forward or reverse
  if((a0 > 490.0) && (a0 < 530.0))
  {
    Serial.println("forward / reverse");
    rpmL = map(a1,0.0, 1023.0, MaxRPM, -MaxRPM);
    rpmR = map(a1,0.0, 1023.0, MaxRPM, -MaxRPM);
    
  }
  // curve to the right
  else if ((a0 >= 531.0) && (a0 <= 771.0))
  {
    Serial.println("curve right");
    rpmL = map(a1,0, 261.0, ThirdMaxRPM, QuatMaxRPM);
    scale_a0 = map(a0,531.0,771.0,100.0,0.0);
    rpmR = rpmL*scale_a0/100.0;
    //Serial.println(scale_a0);
    
  }
  // turn to the right
  else if ((a0 > 771.0) && (a0 <= 1023.0))
  {
    Serial.println("turn right");
    rpmL = map(a1,261.0, 521, QuatMaxRPM, ThirdMaxRPM);
    scale_a0 = map(a0,772.0,1023.0,0.0,-100.0);
    rpmR = rpmL*scale_a0/100.0;
  }
  // curve to the left
  else if((a0 <= 489.0) && (a0 >= 245.0))
  {
    Serial.println("curve left");
    rpmR = map(a1,0.0, 261.0, ThirdMaxRPM, QuatMaxRPM);
    scale_a0 = map(a0, 489.0, 245.0, 100.0, 0.0);
    rpmL = rpmR*scale_a0/100.0;
    
  }
    // turn to the left
  else if((a0 <= 244.0) && (a0 >= 0.0))
  {
    Serial.println("turn left");
    rpmR = map(a1,261.0, 521.0, QuatMaxRPM, ThirdMaxRPM);
    scale_a0 = map(a0, 245.0, 0.0, 0.0, -100.0);
    rpmL = rpmR*scale_a0/100.0;
    
  }
  // Set zero threshold
  if ((rpmL < 3.0) && (rpmL > -3.0)){
    rpmL = 0.0;
    
  }
   if ((rpmR < 3.0) && (rpmR > -3.0)){
    rpmR = 0.0;
    
  }
  //Serial.print("rpmL ");Serial.println(rpmL);
  //Serial.print("rpmR ");Serial.println(rpmR);

  Output[0] = rpmL;
  Output[1] = rpmR;
  
  //delay(100);
  
}

void setup() {
  Serial.begin(57600);
  
  // baudrate of wheel's esc must be 9600
  while(!(BW.serialBegin(9600)));
  // initialize the wheel's esc
  BW.Init();

}



void loop() {

  GetJoyCommand(RPM);

  BW.DriveWheels(RPM[0],RPM[1]);
  //BW.DriveWheels(80.0,80.0);

  Serial.print("rpmL ");Serial.println(RPM[0]);
  Serial.print("rpmR ");Serial.println(RPM[1]);

  
  

}
