// PID essentials

double rightLeftTicksDiff()
{
     
   disableInterrupt(encoderPinLeft);
   disableInterrupt(encoderPinRight);

   double ticksDifference = (encoderPinRightTicks - encoderPinLeftTicks)/1.0;
   
   enableInterrupt(encoderPinLeft, ISREncoderLeft_CHANGE, RISING);
   enableInterrupt(encoderPinRight, ISREncoderRight_CHANGE, RISING);

   return ticksDifference;
}

void resetEncoder()
{
   encoderPinLeftTicks = 0;
   encoderPinRightTicks = 0;
}

void ISREncoderLeft_CHANGE()
{
  encoderPinLeftTicks++;
}

void ISREncoderRight_CHANGE()
{
  encoderPinRightTicks++;
}
