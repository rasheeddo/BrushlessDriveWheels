/**************************************************************************
* A simple library to communicate and send command to Brushless Wheels UGV
* by Rasheed Kittinanthapanya
***************************************************************************/

#ifndef BrushlessWheels_H
#define BrushlessWheels_H

#include "Arduino.h"
#include <HardwareSerial.h> //<SoftwareSerial.h>


class BrushlessWheels
{
public:
    BrushlessWheels(HardwareSerial *ser); //BrushlessWheels(SoftwareSerial*);
    //class constructor to initialize baudrate setting 

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// Data Converting function /////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void Int16ToByteData(unsigned int Data, unsigned char StoreByte[2]);
    // Int16ToByteData: split an unsigned int into 2 bytes data
    // unsigned int stores 16 bits as an integer
    // Pass by reference using array StoreByte[2], this is output of the function

    unsigned int RPMToRaw(float rpm);
    // RPMToRaw: convert rotation per minute value to raw value
    // input is a float number between -MaxRPM to MaxRPM, minus sign means reverse rotation
    // output is a value from 0 to 3200 for forward rotation and 32769 to 35968 for reverse rotation

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////// Hand Shake with ESC //////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool serialBegin(uint32_t baudrate);

    void Init();
    // Init: initialize by do waitUntilFourByte, ESCHandShake and zeroSpeed
    // MUST DO inside the setup() loop on your sketch

    void waitUntilFourZero();
    // waitUntilFourZero: when start, ESC will send four bytes of zero then wait for our controller to response
    // if our control received four bytes of zero, then it will allow next command to run after

    void ESCHandShake();
    // ESCHandShake: from hack, we need to send 17bytes of "Hand-Shake" style 20times with specific delay

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////// Drive function ////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void zeroSpeed();
    // zeroSpeed: set motor to zero rpm, this will be used when start after hand shake

    void DriveWheels(float rpm1, float rpm2);
    // DriveWheels: drive both wheels with desired rpm, + for forward - for reverse rotations

    void DriveByBytes(unsigned char hi_byte, unsigned char lo_byte);


private:

    HardwareSerial * _ser; //SoftwareSerial * _ser;

    bool startTick = true;
    unsigned char ReadByte;
    unsigned char Reply[6];
    int count;
    int i = 0;
    bool ReadOK = false;

    float MaxRPM;
    unsigned int RawInt1;
    unsigned int RawInt2;
    unsigned char Motor1SpeedByte[2];
    unsigned char Motor2SpeedByte[2];

    byte Header1 = 0x02;
    byte Header2 = 0x09;

    unsigned char InitHeader1;
    unsigned char InitHeader2;
    unsigned char ForwardAcc;
    unsigned char ForwardDelay;
    unsigned char BrakeDis;
    unsigned char TurnAcc;
    unsigned char TurnDelay;
    unsigned char AccTimeOfStart;
    unsigned char SenRocker;
    unsigned char UnderVolt1;
    unsigned char UnderVolt2;
    unsigned char StartSpeed;
    unsigned char DriveMode;
    unsigned char PhaseLMotor;
    unsigned char PhaseRMotor;
    unsigned char MotorConfig;
    unsigned char Extra_byte; // Unknown extra byte
    unsigned char InitCheckSum;

    float max_rpm = 136.0;

    unsigned long last_drive_stamp = millis();

};

#endif
