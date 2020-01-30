boolean receiveCommand() {

  if (Serial.available() > 0 ) {
    command = Serial.read();

    if (command != 10  ) {
      return true;
    }
  }
  return false;
}

void executeCommand() {
  if (command == 74) { // rotate left - J
    rotateLeft(1);
    //caliAngle();
    sendIRtoPC();
  } else if (command == 75) { // 180 degree turn left - K
    rotateRight(2);
    sendIRtoPC();
  } else if (command == 76) { // rotate right - L
    rotateRight(1);
    sendIRtoPC();
  } else if (command == 71) { // calibrate top right wall hugging  G
    caliFront();
    Serial.println("cali:done");
    // note that cali:done command is sent
  }else if (command == 72) { // calibrate top right wall hugging  H
    caliAngle();
    // note that cali:done command is sent
  } else if (command == 73) { // calbirate right way hugging - I
    caliRight();
    Serial.println("cali:done");
  } else if (command == 82) { // Send sensor reading - R
    sendIRtoPC();
  }
  // else if (command > 0 || command <= 15 && ((command != 74) || (command != 75) || (command != 76) || (command != 72))) { // 1 grid to 15 grids
  //   forward((command));
  //   sendIRtoPC();
  // }
}
