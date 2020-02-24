void goStraightEX() {
  double orientation = 0; 
  strght_trig++;
  
  //Temporary variable for control system(power)
  double power = 200;
  double leftCoeff = 1.05;
  double powerLeft = leftCoeff * power; 
  double powerRight = power;
  double diffValue = 0;
  double correction = 0;
  
  //Clear the current interupt variable
  resetEncoder();

  //Distance
   double distance = 300;
  
   //PID
   PID PID_straightEX(&diffValue, &correction, &orientation, kpStraightEX, kiStraightEX, kdStraightEX, DIRECT);
   PID_straightEX.SetMode(AUTOMATIC);
   PID_straightEX.SetSampleTime(sampleTime / 2);
   PID_straightEX.SetOutputLimits(-255, 255);

  while ((encoderPinLeftTicks + encoderPinRightTicks) / 2 < distance) {
    if (PID_straightEX.Compute()) {
      diffValue = leftRightTicksDiff();
      powerLeft = leftCoeff * power + correction;
      powerRight = power - correction;
      if ((encoderPinRightTicks + encoderPinLeftTicks) / 2 + 100 >= distance) {
        powerRight = powerRight / 2.0;
        powerLeft = powerLeft / 2.0;
      }
    }
    md.setSpeeds((int)powerRight, (int)powerLeft);
    // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
    // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  }
  brakeEX();
  // diffValue = leftRightTicksDiff();
  // Serial.println(String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
  // Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  // Serial.println("OK");
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
    Serial.println("     " + String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
    Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  }
  brakeFP();
  diffValue = leftRightTicksDiff();
  Serial.println("     " + String(encoderPinLeftTicks) +" | "+ String(encoderPinRightTicks));
  Serial.println(String(diffValue) +":dif | correction:"+ String(correction));
  Serial.println("OK");
}
