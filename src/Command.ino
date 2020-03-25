boolean receiveCommand() {
  if (Serial.available() > 0) {
    command = Serial.read(); // int
    return true;
  }
  return false;
}

void executeCommand() {
  if (command == 'l') {
    rotateLeft(1);
  } else if (command == 'r') {
    rotateRight(1);
  } else if (command == 'f') {
    goStraightEX();
  } else if (command == 'b') {
    goBackFP(1);
  } else if (command == 'c') {
    caliAngle();
  } else if (command == 's') {
    // nothing
  } else if ('A' <= command && command <= 'Z') {
    goStraightFP(command - 'A' + 1);
    return;
  } else {
    return;
  }
  sendIRtoPC();
  Serial.flush();
}
