// Brakes
void brake() {
  md.setBrakes(380, 400);
}

void brakeEX() {
  md.setSpeeds(0, 0);
  md.setM1Brake(400);
  md.setM2Brake(400);
}

void brakeFP() {
  md.setSpeeds(0, 0);
  md.setM1Brake(360);
  md.setM2Brake(380);
}

void rotateLeft(int grid) {
  double power = 300;
  double powerLeft = power;
  double powerRight = power;
  double orientation = 0;
  double diffValue = 0;
  double correction = 0;
  double tickTarget = 0;

  resetEncoder(); // Clear Tick Counts
  if (grid == 0){
    tickTarget = 1;
  } else if (grid == 1) {
    tickTarget = 390;
  } else if (grid == 2) {
    tickTarget = 815;
  }

  PID PID_left(&diffValue, &correction, &orientation, kpLeft, kiLeft, kdLeft, DIRECT);
  PID_left.SetMode(AUTOMATIC);
  PID_left.SetSampleTime(sampleTime);
  PID_left.SetOutputLimits(-255, 255);

  while ((encoderPinRightTicks + encoderPinLeftTicks) / 2 < tickTarget) {
    if (PID_left.Compute()) {
      diffValue = leftRightTicksDiff();
      powerRight = power - correction;
      powerLeft =  power + correction;
      if ((encoderPinRightTicks + encoderPinLeftTicks) / 2 + 150 >= tickTarget) {
        powerRight = powerRight * 0.75;
        powerLeft = powerLeft * 0.75;
      }
      md.setSpeeds((int)powerRight, -(int)powerLeft);
    }
    // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
    // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  }
  brake();
  // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
  // diffValue = leftRightTicksDiff();
  // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  // Serial.println("OK");
}

void rotateRight(double grid) {
  double power = 300;
  double powerLeft = power;
  double powerRight = power;
  double orientation = 0;
  double diffValue = 0;
  double correction = 0;
  double tickTarget = 0;

  resetEncoder(); // Clear Tick Counts
  if (grid == 0){
    tickTarget = 1;
  } else if (grid == 1) {
    tickTarget = 390;
  } else if (grid == 2) {
    tickTarget = 815;
  }

  PID PID_right(&diffValue, &correction, &orientation, kpRight, kiRight, kdRight, DIRECT);
  PID_right.SetMode(AUTOMATIC);
  PID_right.SetSampleTime(sampleTime);
  PID_right.SetOutputLimits(-255, 255);

  while ((encoderPinRightTicks + encoderPinLeftTicks) / 2 < tickTarget) {
    if (PID_right.Compute()) {
      diffValue = leftRightTicksDiff();
      powerRight = power - correction;
      powerLeft =  power + correction;
      if ((encoderPinRightTicks + encoderPinLeftTicks) / 2 + 150 >= tickTarget) {
        powerRight = powerRight * 0.75;
        powerLeft = powerLeft * 0.75;
      }
      md.setSpeeds((int)-powerRight, (int)powerLeft);
    }
    // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
    // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  }
  brake();
  // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
  // diffValue = leftRightTicksDiff();
  // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  // Serial.println("OK");
}

void goBackFP(int grid) {
  double power = 380;
  if (grid == 0) {
    power = 250;
  }
  double powerLeft = power;
  double powerRight = power;
  double diffValue = 0;
  double correction = 0;
  double orientation = 0;

  resetEncoder();

  //Distance
  double distance = 0;
  switch(grid) {
    case 0: distance = 8; break;
    case 1: distance = 297.16; break;
    case 2: distance = 586.68; break;
    case 3: distance = 898.6; break;
    case 4: distance = 1200.29; break;
    case 5: distance = 1505.56; break;
    case 6: distance = 1758.86; break;
    case 7: distance = 2068.82; break;
    case 8: distance = 2350.04; break;
    case 9: distance = 2665.76; break;
    case 10: distance = 2966.76; break;
    case 11: distance = 3266.43; break;
    case 12: distance = 3555.85; break;
    case 13: distance = 3851.83; break;
    case 14: distance = 4171.40; break;
    case 15: distance = 4450.41; break;
    case 16: distance = 4803.03; break;
    default: distance = 304.8799 * grid; 
  } 

  PID PID_backFP(&diffValue, &correction, &orientation, kpStraightFP, kiStraightFP, kdStraightFP, DIRECT);
  PID_backFP.SetMode(AUTOMATIC);
  PID_backFP.SetSampleTime(sampleTime / 2);
  PID_backFP.SetOutputLimits(-255, 255);

  while ((encoderPinLeftTicks + encoderPinRightTicks) / 2 < distance) {
    if (PID_backFP.Compute()) {
      diffValue = leftRightTicksDiff();
      powerRight = power - correction;
      powerLeft = power + correction;
      md.setSpeeds(-(int)powerRight, -(int)powerLeft);
    }
  }
  brakeFP();
}
