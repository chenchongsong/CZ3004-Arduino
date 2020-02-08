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

double kpStraightEX = 0.5; //1.68; //1.3 
double kiStraightEX = 0.0; //0.2;
double kdStraightEX = 0.0; //0.6; //0.5

double kpStraightFP = 2.5; //2.8
double kiStraightFP = 0.0;
double kdStraightFP = 0.0;

double kpLeft = 0.7;
double kiLeft = 0.5;
double kdLeft = 0.2;
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
  delay(1000);
  checkRawValues();
  // Serial.println(irArr1);
  // Serial.println("irR2");
  // Serial.println(analogRead(irR2));
  // Serial.println("irR3");
  // Serial.println(analogRead(irR3));
  // Serial.print("\nA3 = \n");
  // Serial.print(analogRead(A3));
  // Serial.print("\nA2 = \n");
  // Serial.print(analogRead(A2));
  // Serial.print("\nA5 = \n");
  // Serial.print(analogRead(A5));

  // Serial.println("OK");
  // goStraightFP(10);
  // delay(10000);
  // goBackFP(10);
  // delay(10000);

  // goStraightEX();
  // delay(1000);
   
  // rotateLeft(8); //1 = 90deg, 4 = 360deg, 8 = 720deg, 12 = 1080 deg
  // rotateRight(4);
  // delay(200);
  // rotateRight(4);

  // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));

  // // put your main code here, to run repeatedly:
  // if (receiveCommand())
  // {
  //   executeCommand();
  // }
}
