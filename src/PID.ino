// PID essentials

double leftRightTicksDiff()
{
  disableInterrupt(encoderPinLeft);
  disableInterrupt(encoderPinRight);

  double ticksDifference = encoderPinLeftTicks - encoderPinRightTicks;
  enableInterrupt(encoderPinLeft, ISREncoderLeft_CHANGE, RISING);
  enableInterrupt(encoderPinRight, ISREncoderRight_CHANGE, RISING);

  return ticksDifference;
}

void resetEncoder()
{
  disableInterrupt(encoderPinLeft);
  disableInterrupt(encoderPinRight);
  encoderPinLeftTicks = 0;
  encoderPinRightTicks = 0;
  enableInterrupt(encoderPinLeft, ISREncoderLeft_CHANGE, RISING);
  enableInterrupt(encoderPinRight, ISREncoderRight_CHANGE, RISING);
}

void ISREncoderLeft_CHANGE()
{
  encoderPinLeftTicks++;
}

void ISREncoderRight_CHANGE()
{
  encoderPinRightTicks++;
}
