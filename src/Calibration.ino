void caliDistance() {
  bool tried_front = false;
  bool tried_back = false;
  float dist2 = 4.55;
  float dist3 = 5.0;
  float dist4 = 3.7;
  float error = 0.5;
  int cDsamples = 7;
  irSamples(cDsamples);

  while(((abs(median(irArr2,cDsamples)- dist2) > error) ||
         (abs(median(irArr3,cDsamples)- dist3) > error) ||
         (abs(median(irArr4,cDsamples)- dist4) > error))
        && not(tried_front && tried_back)){
    if (median(irArr2,cDsamples) < dist2 || median(irArr3,cDsamples) < dist3 || median(irArr4,cDsamples) < dist4) {
      goBackFP(0);
      irSamples(cDsamples);
      tried_front = true;
    }
    else if (median(irArr2,cDsamples) > dist2 && median(irArr3,cDsamples) > dist3 && median(irArr4,cDsamples) > dist4) {
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

  float ir_diff = median(irArr2, cFsamples) - median(irArr4, cFsamples);
  float ideal = 0.6;   // larger value : left
  bool tried_left = false;
  bool tried_right = false;

  if (median(irArr2, cFsamples) > 9.5 || median(irArr4, cFsamples) > 9.5) {
    return;
  }

  //using FL and FR from front sensors
  while ((abs(ir_diff - ideal) > 0.2) && not(tried_left && tried_right)) {
    if (ir_diff - ideal < 0) {
      rotateLeft(0);
      tried_left = true;
    }
    else if (ir_diff - ideal > 0) {
      rotateRight(0);
      tried_right = true;
    }
    irSamples(cFsamples);
    ir_diff = median(irArr2,cFsamples) - median(irArr4,cFsamples);
  }
}

void caliAngle() {
  caliFront();
  caliDistance();
  caliFront();
}
