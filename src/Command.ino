boolean receiveCommand() {
  if (Serial.available() > 0) {
    command = Serial.read(); // int
    Serial.println("received:" + String((char)command));
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
    goStraightFP(1);
  } else if (command == 'b') {
    goBackFP(1);
  } else if (command == 'c') {
    caliAngle();
  } else if (command == 's') {
    sendIRtoPC();
  }
  Serial.flush();
}
