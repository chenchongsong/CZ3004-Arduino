void goStraightEX() {
  double orientation = -8; // negative means left 
  strght_trig++;
  
  //Temporary variable for control system(power)
  double power = 400;
  double rightCoeff = 0.92;
  double powerLeft = power;
  double powerRight = rightCoeff * power;
  double diffValue = 0;
  double correction = 0;
  
  //Clear the current interupt variable
  resetEncoder();

  //Distance
  double distance = 285;
  
  //PID
  PID PID_straightEX(&diffValue, &correction, &orientation, kpStraightEX, kiStraightEX, kdStraightEX, DIRECT);
  PID_straightEX.SetMode(AUTOMATIC);
  PID_straightEX.SetSampleTime(sampleTime / 4);
  PID_straightEX.SetOutputLimits(-255, 255);

  while (encoderPinLeftTicks < distance) {
    if (PID_straightEX.Compute()) {
      diffValue = leftRightTicksDiff();
      powerLeft = power + correction;
      powerRight = rightCoeff * power - correction;
      if ((encoderPinRightTicks + encoderPinLeftTicks) / 2 + 100 >= distance) {
        powerRight = powerRight * 0.5;
        powerLeft = powerLeft * 0.5;
      }
    }
    md.setSpeeds((int)powerRight, (int)powerLeft);
    // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
    // Serial.println("    " + String(diffValue) +":dif | correction:"+ String(correction));
  }
  brakeEX();
  // Serial.println("dummy1ex" + String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
  
  if (encoderPinLeftTicks > 250.0) return;

  // Half Grid Exception Handling
  delay(500);
  distance = distance - encoderPinLeftTicks;
  orientation = 0 - diffValue;
  diffValue = 0;
  correction = 0;
  resetEncoder();
  PID PID_COM(&diffValue, &correction, &orientation, kpStraightEX, kiStraightEX, kdStraightEX, DIRECT);
  PID_COM.SetMode(AUTOMATIC);
  PID_COM.SetSampleTime(sampleTime / 2);
  PID_COM.SetOutputLimits(-255, 255);

  while (encoderPinLeftTicks < distance) {
    if (PID_COM.Compute()) {
      diffValue = leftRightTicksDiff();
      powerLeft = power + correction;
      powerRight = rightCoeff * power - correction;
      if ((encoderPinRightTicks + encoderPinLeftTicks) / 2 + 100 >= distance) {
        powerRight = powerRight * 0.75;
        powerLeft = powerLeft * 0.75;
      }
    }
    md.setSpeeds((int)powerRight, (int)powerLeft);
  }
  brakeEX();
  // Serial.println("After Half Grid Handling");
  // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
}

void goStraightFP(int grid) {
  strght_trig++;
  
  double power = 380;
  double powerLeft = power;
  double powerRight = power;
  double diffValue = 0;
  double correction = 0;
  double orientation = 0;

  resetEncoder(); // Clear Tick Counts

  //Distance
  double distance = 0;
  switch(grid) {
    case 0: distance = 8; break;        // for calibration
    case 1: distance = 279; break;      // tested
    case 2: distance = 576.68; break;   // tested
    case 3: distance = 868.6; break;    // tested
    case 4: distance = 1180.29; break;  // tested
    case 5: distance = 1485.56; break;  // tested
    case 6: distance = 1758.86; break;  // tested
    case 7: distance = 2068.82; break;  // tested
    case 8: distance = 2350.04; break;
    case 9: distance = 2665.76; break;
    case 10: distance = 2966.76; break;
    case 11: distance = 3266.43; break;
    case 12: distance = 3555.85; break;
    case 13: distance = 3851.83; break;
    case 14: distance = 4171.40; break;
    case 15: distance = 4450.41; break;
    case 16: distance = 4783.03; break;
    case 17: distance = 5083.03; break;
    default: distance = 303.8799 * grid;
  } 
  
  PID PID_straightFP(&diffValue, &correction, &orientation, kpStraightFP, kiStraightFP, kdStraightFP, DIRECT);
  PID_straightFP.SetMode(AUTOMATIC);
  PID_straightFP.SetSampleTime(sampleTime / 2);
  PID_straightFP.SetOutputLimits(-255, 255);
  delay(100);
 
  while ((encoderPinLeftTicks + encoderPinRightTicks) / 2 < distance) {
    if (PID_straightFP.Compute()) {
      diffValue = leftRightTicksDiff();
      powerRight = power - correction;
      powerLeft = power + correction;
    }
    md.setSpeeds((int)powerRight, (int)powerLeft);

    if (distance > 140 &&
        (encoderPinRightTicks + encoderPinLeftTicks) / 2 + 140 >= distance && 
        readIR2() < 5.55 &&
        readIR3() < 6.0 &&
        readIR4() < 4.7) {
      break;
    }
    // Serial.println("     " + String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
    // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  }
  brakeFP();
  // diffValue = leftRightTicksDiff();
  // Serial.println("     " + String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
  // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  // Serial.println("OK");
}
