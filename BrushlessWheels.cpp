/**************************************************************************
* A simple library to communicate and send command to Brushless Wheels UGV
* by Rasheed Kittinanthapanya
***************************************************************************/

#include "Arduino.h"
#include "BrushlessWheels.h"

BrushlessWheels::BrushlessWheels(SoftwareSerial *ser)
{
  _ser = ser;
  // Initialized of 17 bytes
  InitHeader1 = 0x01;     // always constant
  InitHeader2 = 0x11;     // always constant
  ForwardAcc = 0x32;      // 0x00 to 0x64   [0-100]   An acceleration when changing speed value
  ForwardDelay = 0x00;    // 0x00 t0 0x05   [0-5]     A delay before start to go
  BrakeDis = 0x0A;        // 0x00 to 0x64   [0-100]   Brake distance, should be as short as possible (rigid brake)
  TurnAcc = 0x32;         // 0x00 to 0x64   [0-100]   Turning acceleration, when two wheels has reverse direction
  TurnDelay = 0x01;       // 0x00 t0 0x05   [0-5]     A delay before start turning
  AccTimeOfStart = 0x00;  // 0x00 to 0x32   [0-50]    increase this will make wheels slower
  SenRocker = 0x83;       // Don't need to change     this is for curving motion, we have our own calculation.
  UnderVolt1 = 0x14;      // 0x12 -> 18.0V, 0x13 -> 19.0V, 0x14 -> 20.0V, 0x15 -> 21.0V, 0x16 -> 22.0V
  UnderVolt2 = 0x05;      // 0x01 to 0x09 -> 0.1V tp 0.9V
  StartSpeed = 0x0A;      // 0x00 to 0x64   [0-100]   starting speed if too high you will hear some cogging sound out from gear, set not too high
  DriveMode = 0x01;       // 0x01 is Sine wave, 0x00 is square wave. Don't need to change, square wave seems not working well...
  PhaseLMotor = 0x03;     // Don't need to change     not sure what is this, so leave it alone 
  PhaseRMotor = 0x04;     // Don't need to change     not sure what is this, so leave it alone
  MotorConfig = 0x07;     // Don't need to change     This is about choosing which wheel will be reverse
  InitCheckSum = InitHeader1 + InitHeader2 + ForwardAcc + ForwardDelay + BrakeDis + TurnAcc + TurnDelay + AccTimeOfStart + SenRocker 
                  + UnderVolt1 + UnderVolt2 + StartSpeed + DriveMode + PhaseLMotor + PhaseRMotor + MotorConfig;
  
}

bool BrushlessWheels::serialBegin(uint32_t baudrate){

  // this motor's esc needs 9600 bps baudrate
  if (baudrate != 9600){

    return 0;
  }
  else{
    _ser->begin(baudrate);
    return 1;
  }
  
}

void BrushlessWheels::Init()
{
  waitUntilFourZero();                                  // Wait ESC to response and do a hand-shake
  delay(219);                                           // DON'T change this delay, it's from hacking
  ESCHandShake();

  for (i=1;i<10;i++)                                    // start the wheels from zero rpm before drive
  {
    zeroSpeed();
  }
}

void BrushlessWheels::Int16ToByteData(unsigned int Data, unsigned char StoreByte[2])
{
  // unsigned int can store 16 bit int 
  StoreByte[0] = (Data & 0xFF00) >> 8;                  //High byte, most right of HEX
  StoreByte[1] = (Data & 0x00FF);                       //Low byte, most left of HEX
}

unsigned int BrushlessWheels::RPMToRaw(float rpm)
{
  int raw_int;
  unsigned int out_raw;

    // map rpm to raw value
    raw_int = (int)map(rpm, 0.0, 144.0, 0, 3200);
  
    // In case of negative number, shift mapped number from 32768 to 35968 (for 0.0 to -146.0)
    if (rpm < 0.0)
    {
      out_raw = 32768 + abs(raw_int);
      }
    // In case of positive number, use the mapped number for unsigned int type
    else
    {
      out_raw = raw_int;
      }
  
  return out_raw;
}

void BrushlessWheels::waitUntilFourZero()
{
  while (startTick == true)
  {

    while (_ser->available() > 0)
      {  

      ReadByte = _ser->read();
      Reply[i] = ReadByte;
      i++;
      ReadOK = true;
      delayMicroseconds(2000);                      // This delay will wait for incoming byte to complete 6 bytes as
     
      }

   if (ReadOK == true){
  
    if ( (Reply[0] == 0) && (Reply[1] == 0) && (Reply[2] == 0) && (Reply[3] == 0 ))
    {
      //Serial.println("Get start byte");
      startTick = false;
    }
    
    i = 0;    // reset i 
    //memset(Reply, 0, sizeof(Reply));
    ReadOK = false;
    
    }
  }
  
}

void BrushlessWheels::ESCHandShake()
{
  //Serial.println("Here");
  for (i=1;i<=20;i++)
  {  
    _ser->write(InitHeader1);
    _ser->write(InitHeader2);
    _ser->write(ForwardAcc);
    _ser->write(ForwardDelay);
    _ser->write(BrakeDis);
    _ser->write(TurnAcc);
    _ser->write(TurnDelay);
    _ser->write(AccTimeOfStart);
    _ser->write(SenRocker);
    _ser->write(UnderVolt1);
    _ser->write(UnderVolt2);
    _ser->write(StartSpeed);
    _ser->write(DriveMode);
    _ser->write(PhaseLMotor);
    _ser->write(PhaseRMotor);
    _ser->write(MotorConfig);
    _ser->write(InitCheckSum);
    _ser->flush();
    
    if (i == 1){
      delayMicroseconds(300);                              // DON'T change this delay, it's from hacking
    }
    else{

      delay(14);                                           // DON'T change this delay, it's from hacking
    }
  }
}

void BrushlessWheels::zeroSpeed()
{
    _ser->write(Header1);      
    _ser->write(Header2);
    _ser->write((unsigned char)0x00);            // Motor1 speed hibyte
    _ser->write((unsigned char)0x00);            // Motor1 speed lobyte
    _ser->write((unsigned char)0x00);            // Motor2 speed hibyte
    _ser->write((unsigned char)0x00);            // Motor2 speed lobyte
    _ser->write((unsigned char)0xB4);            // Mode hibyte (don't care)
    _ser->write((unsigned char)0x00);            // Mode lobyte (don't care)
    _ser->write((unsigned char)0xBF);            // Check sum
    _ser->flush();
    delay(23);                      // DON'T change this delay, it's from hacking
  
}

void BrushlessWheels::DriveWheels(float rpm1, float rpm2)
{

  RawInt1 = RPMToRaw(rpm1);
  RawInt2 = RPMToRaw(rpm2);
  Int16ToByteData(RawInt1,Motor1SpeedByte);
  Int16ToByteData(RawInt2,Motor2SpeedByte);
  
  byte Motor1hibyte = Motor1SpeedByte[0];
  byte Motor1lobyte = Motor1SpeedByte[1];
  
  byte Motor2hibyte = Motor2SpeedByte[0];
  byte Motor2lobyte = Motor2SpeedByte[1];
  
  byte Modehibyte = 0x00;    // don't care 
  byte Modelobyte = 0x00;    // don't care
  byte CheckSum = Header1 + Header2 + Motor1hibyte + Motor1lobyte + Motor2hibyte + Motor2lobyte + Modehibyte + Modelobyte;
  
  
    _ser->write(Header1);
    _ser->write(Header2);
    _ser->write(Motor1hibyte);
    _ser->write(Motor1lobyte);
    _ser->write(Motor2hibyte);
    _ser->write(Motor2lobyte);
    _ser->write(Modehibyte);
    _ser->write(Modelobyte);
    _ser->write(CheckSum);
    _ser->flush();
   delay(23);                      // DON'T change this delay, it's from hacking

  
}