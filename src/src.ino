#include "PID_v1.h"
#include "RunningMedian.h"

#include "DualVNH5019MotorShield.h"
#include "EnableInterrupt.h"
#include "SharpIR.h"

#define M1E1Right 3
#define M2E2Left 13

#define irR1 A0
#define irR2 A1
#define irR3 A2
#define irR4 A3
#define irR5 A4
#define irR6 A5

/// PID constant variables
double sampleTime = 100;

double kpStraightEX = 0.3; // larger -> left_tick larger -> turn right
double kiStraightEX = 0.0; //0.2;
double kdStraightEX = 0.0; //0.6; //0.5

double kpStraightFP = 2.5; //2.8
double kiStraightFP = 0.0;
double kdStraightFP = 0.0;

double kpLeft = 0.7;
double kiLeft = 0.0;
double kdLeft = 0;
double kpRight = kpLeft;
double kiRight = kiLeft;
double kdRight = kdLeft;

const int encoderPinLeft = 3;
const int encoderPinRight = 11;

//Interrupt
double encoderPinLeftTicks = 0;
double encoderPinRightTicks = 0;

DualVNH5019MotorShield md;

// Initialise  sensors
SharpIR SharpIRR1(irR1, 20150);
SharpIR SharpIRR2(irR2, 1080);
SharpIR SharpIRR3(irR3, 1080);
SharpIR SharpIRR4(irR4, 1080);
SharpIR SharpIRR5(irR5, 1080);
SharpIR SharpIRR6(irR6, 1080);

float distanceCMR1;
float distanceCMR2;
float distanceCMR3;
float distanceCMR4;
float distanceCMR5;
float distanceCMR6;

// read 50 times and store into int array then it returns the median
float irArr1[50];
float irArr2[50];
float irArr3[50];
float irArr4[50];
float irArr5[50];
float irArr6[50];

// For receiving commands from Rpi
int command;
int strght_trig = 0;

void setup() {
  //Debug
  // analogReference(EXTERNAL);
  Serial.begin(9600);
  //Pin Initialization
  pinMode(encoderPinLeft, INPUT);
  pinMode(encoderPinRight, INPUT);
  // digitalWrite(encoderPinLeft, LOW);
  // digitalWrite(encoderPinRight, LOW);
  
  md.init();
  pinMode(irR1, INPUT);
  pinMode(irR2, INPUT);
  pinMode(irR3, INPUT);
  pinMode(irR4, INPUT);
  pinMode(irR5, INPUT);
  pinMode(irR6, INPUT);

  enableInterrupt(encoderPinLeft, ISREncoderLeft_CHANGE, RISING);
  enableInterrupt(encoderPinRight, ISREncoderRight_CHANGE, RISING);
}

void loop() {
  delay(3000);
  
  double power = 250;
  double powerLeft = power;
  double powerRight = power;
  double diffValue = 0;
  double correction = 0;
  double orientation = 0;

  resetEncoder(); // Clear Tick Counts

  double distance = 1758.86;; // 6 grids
  double tmpLeftTicks = 0;
  double tmpRightTicks = 0;
  
  PID PID_straightFP(&diffValue, &correction, &orientation, kpStraightFP, kiStraightFP, kdStraightFP, DIRECT);
  PID_straightFP.SetMode(AUTOMATIC);
  PID_straightFP.SetSampleTime(sampleTime / 2);
 
  int irsampleSize = 10;
  irSamples(irsampleSize);
  float block_dis = 4.5;

  while ((encoderPinLeftTicks + encoderPinRightTicks) / 2 < distance 
    && median(irArr2, irsampleSize) > block_dis
    && median(irArr3, irsampleSize) > block_dis
    && median(irArr4, irsampleSize) > block_dis) {
    if (PID_straightFP.Compute()) {
      diffValue = leftRightTicksDiff();
      powerRight = power - correction;
      powerLeft = power + correction;
    }
    md.setSpeeds((int)powerRight, (int)powerLeft);
    irSamples(irsampleSize);
  }
  brakeFP();

  disableInterrupt(encoderPinLeft);
  disableInterrupt(encoderPinRight);
  tmpLeftTicks = encoderPinLeftTicks;
  tmpRightTicks = encoderPinRightTicks;
  enableInterrupt(encoderPinLeft, ISREncoderLeft_CHANGE, RISING);
  enableInterrupt(encoderPinRight, ISREncoderRight_CHANGE, RISING);

  rotateLeft(1);
  goStraightFP(3);
  rotateRight(1);
  goStraightFP(4);
  rotateRight(1);
  goStraightFP(3);
  rotateLeft(1);

  // restore
  disableInterrupt(encoderPinLeft);
  disableInterrupt(encoderPinRight);
  encoderPinLeftTicks = tmpLeftTicks;
  encoderPinRightTicks = tmpRightTicks;
  enableInterrupt(encoderPinLeft, ISREncoderLeft_CHANGE, RISING);
  enableInterrupt(encoderPinRight, ISREncoderRight_CHANGE, RISING);

  while ((encoderPinLeftTicks + encoderPinRightTicks) / 2 < distance) {
    if (PID_straightFP.Compute()) {
      diffValue = leftRightTicksDiff();
      powerRight = power - correction;
      powerLeft = power + correction;
    }
    md.setSpeeds((int)powerRight, (int)powerLeft);
  }
  brakeFP();

  delay(30000);
}
