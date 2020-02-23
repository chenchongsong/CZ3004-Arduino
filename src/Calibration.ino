// void caliRight() {
//   bool tried_left = false;
//   bool tried_right = false;
//   int cRsamples = 7;
//   irSamples(cRsamples);
//   float ir_diff = (median(irArr5,7)) - median(irArr6,7);

//   while (abs(ir_diff) > 0.4 && not(tried_left && tried_right)) {
//     if (ir_diff < 0.4) {
//       rotateLeft(0);
//       tried_left = true;
//     }
//     else if (ir_diff > 0.4) {
//       rotateRight(0);
//       tried_right = true;
//     }
//     irSamples(cRsamples);
//     ir_diff = (median(irArr5,7)) - median(irArr6,7);
//   }
// }

void caliDistance() {
  bool tried_front = false;
  bool tried_back = false;
  float dist = 5.0;
  float error = 0.4;
  int cDsamples = 7;
  irSamples(cDsamples);
  
  // Serial.println("Prev");
  // Serial.println(median(irArr3, cDsamples));

  while( abs(median(irArr3, cDsamples) - dist) > error
          && not(tried_front && tried_back) ) {
    
    // Serial.println("Center");
    // Serial.println(median(irArr3, cDsamples));

    if (median(irArr2,cDsamples) < dist || median(irArr3,cDsamples) < dist || median(irArr3,cDsamples) < dist) {
      goBackFP(0);
      irSamples(cDsamples);
      tried_front = true;
    }
    else if (median(irArr3,cDsamples) > dist && median(irArr3,cDsamples) > dist && median(irArr3,cDsamples) > dist) {
      goStraightFP(0);
      irSamples(cDsamples);
      tried_back = true;
    }
  }
}

void caliFront() {
  //make robot parallel
  int cFsamples = 7;
  irSamples(cFsamples);
  // Serial.println("Prev");
  // Serial.println(median(irArr2, cFsamples));
  // Serial.println(median(irArr4, cFsamples));

  float ir_diff = median(irArr2, cFsamples) - median(irArr4, cFsamples);
  float ideal = 0.4;
  bool tried_left = false;
  bool tried_right = false;

  //using FL and FR from front sensors
  while ((abs(ir_diff - ideal) > 0.3) && not(tried_left && tried_right)) {
    if (ir_diff < 0) {
      rotateLeft(0);
      tried_left = true;
    }
    else if (ir_diff > 0) {
      rotateRight(0);
      tried_right = true;
    }
    irSamples(cFsamples);
    ir_diff = median(irArr2,cFsamples) - median(irArr4,cFsamples);
  }
}

void caliAngle() {
  caliDistance();
  caliFront();
  caliDistance();
  Serial.println("cali:done");
}
