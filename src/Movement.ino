// Brakes

void brake() {
  md.setSpeeds(0, 0);
  for (int i = 200; i < 400; i += 50) {
    md.setBrakes(i, i);
    delay(10);
  }
  delay(10);
  // md.setBrakes(380, 400);
}

void brakeFP() {
  md.setSpeeds(0, 0);
  for (int i = 200; i < 400; i += 50) {
    md.setBrakes(i, i);
    delay(10);
  }
  delay(10);
  // md.setM1Brake(360); //left
  // md.setM2Brake(380);
}

void forward(int blockstomove) {

  switch (blockstomove)
  {

  case 49: //1
  {
    goStraightEX();
    break;
  }
  case 50:
  {
    goStraightFP(2);
    break;
  }
  case 51:
  {
    goStraightFP(3);
    break;
  }
  case 52:
  {
    goStraightFP(4);
    break;
  }
  case 53:
  {
    goStraightFP(5);
    break;
  }
  case 54:
  {
    goStraightFP(6);
    break;
  }
  case 55:
  {
    goStraightFP(7);
    break;
  }
  case 56:
  {
    goStraightFP(8);
    break;
  }
  case 57:
  {
    goStraightFP(9);
    break;
  }
  case 65:
  {
    goStraightFP(10);
    break;
  }
  case 66:
  {
    goStraightFP(11);
    break;
  }
  case 67:
  {
    goStraightFP(12);
    break;
  }
  case 68:
  {
    goStraightFP(13);
    break;
  }
  case 69:
  {
    goStraightFP(14);
    break;
  }
  case 70:
  {
    goStraightFP(15);
    break;
  }
  default:
    break;
  }
}

// add back, left and right movement here

void rotateRight(double grid) {
  //Temporary variable for control system(power)
  double power = 300;
  double powerLeft = power;
  double powerRight = power;
  double diffValue = 0;
  double correction = 0;
  double orientation = 0;

  //Clear the current interupt variable
  resetEncoder();

  double tickTarget = 369 * grid; //369

  //PID
  PID PID_right(&diffValue, &correction, &orientation, kpRight, kiRight, kdRight, DIRECT);
  PID_right.SetMode(AUTOMATIC);
  PID_right.SetSampleTime(sampleTime);

  // if (grid == 0) {
  //   tickTarget = 1;
  // }
  // else if (strght_trig >= 4 && grid != 2) {
  //   tickTarget = 378;
  //   strght_trig = 0;
  // }
  // else if (strght_trig == 1 && grid != 2) {
  //   tickTarget = 372;
  //   strght_trig = 0; 
  // }

  while ((encoderPinRightTicks + encoderPinLeftTicks) / 2 < tickTarget) {
    if (PID_right.Compute()) {
      diffValue = rightLeftTicksDiff();

      powerRight = power * 0.9969 - correction; 
      powerLeft = power + correction;

      //Execute
      md.setSpeeds(-1 * (int)(powerRight), 1 * (int)powerLeft);
    }
  }
  brake();
}

void rotateLeft(double grid) {
  double orientation = 0;

  //Temporary variable for control system(power)
  double power = 300;
  double powerLeft = power;
  double powerRight = power;
  double diffValue = 0;
  double correction = 0;

  resetEncoder(); // Clear Tick Counts
  double tickTarget = 371 * grid;

  PID PID_left(&diffValue, &correction, &orientation, kpLeft, kiLeft, kdLeft, DIRECT);
  PID_left.SetMode(AUTOMATIC);
  PID_left.SetSampleTime(sampleTime);

  // if (grid == 0 ) {
  //   tickTarget = 1;
  //   strght_trig = 0;
  // }
  // else if (strght_trig == 1 &&  grid != 2) {
  //   tickTarget = 386;
  //   strght_trig = 0;
  // }
  while ((encoderPinRightTicks + encoderPinLeftTicks) / 2 < tickTarget) {
    if (PID_left.Compute()) {
      diffValue = rightLeftTicksDiff();
      powerRight = power - correction;
      powerLeft = 0.915 * power + correction;
      md.setSpeeds((int)powerRight, -(int)powerLeft);
    }
  }
  brake();
}

void goBackFP(int grid) {
  double orientation = 0;

  //Temporary variable for control system(power)
  double power = 380;
  double powerLeft = power;
  double powerRight = power;
  double diffValue = 0;
  double correction = 0;

  //Clear the current interupt variable
  resetEncoder();

  //Distance
  double distance = 0;
  switch (grid)
  {
  case 0:
    distance = 8;
    break;
  case -2:
    distance = 297.16 * 0.707;
    break;
  case 1:
    distance = 297.16;
    break;
  case 2:
    distance = 590.68;
    break;
  case 3:
    distance = 902.6;
    break;
  case 4:
    distance = 1230.29;
    break;
  case 5:
    distance = 1526.56;
    break;
  case 6:
    distance = 1823.86;
    break;
  case 7:
    distance = 2142.82;
    break;
  default:
    distance = 304.8799 * grid;
    break;
  }

  //PID
  PID PID_backFP(&diffValue, &correction, &orientation, kpStraightFP, kiStraightFP, kdStraightFP, DIRECT);
  PID_backFP.SetMode(AUTOMATIC);
  PID_backFP.SetSampleTime(sampleTime / 2);
  //PID_backFP.SetOutputLimits(-1000, 1000);

  //Serial.println(readSensor(Mid_mid));

  while ((distance == -1 || (encoderPinLeftTicks + encoderPinRightTicks) / 2 < distance))
  {
    if (PID_backFP.Compute())
    {
      diffValue = rightLeftTicksDiff();

      powerRight = power - correction;
      powerLeft = power + correction;

      md.setSpeeds(-1 * (int)powerRight, -1 * (int)powerLeft);
    }
  }
  brakeFP();
}
