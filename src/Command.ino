boolean receiveCommand() {
  if (Serial.available() > 0) {
    command = Serial.read(); // int
    Serial.println("received:" + String((char)command));
    return true;
  }
  return false;
}

void executeCommand() {
  if (command == 'l') { // rotate left - J
    rotateLeft(1);
    // sendIRtoPC();
  } else if (command == 'r') { // rotate right - L
    rotateRight(1);
    // sendIRtoPC();
  } else if (command == 'f') {
    goStraightFP(1);
    // sendIRtoPC();
  }
  Serial.flush();
}
