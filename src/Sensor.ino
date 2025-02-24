float ir_valR3;
float ir_valR4;
float ir_valR5;
float ir_valR6; 

int irsampleSize = 10;

//Raw values to OFFSET
// LEFT LONG 
float readIR1() {
  return 4 + 60.374 * pow(map(analogRead(irR1), 0, 1023, 0, 5000) / 1000.0, -1.16) - 0;
}// 5,10,18,27,37,53,70

// FRONT LEFT
float readIR2() {
  return 27.728 * pow(map(analogRead(irR2), 0, 1023, 0, 5000) / 1000.0, -1.2045) -5.5;
}//5,15,25,35

// FRONT MIDDLE 
float readIR3() {
  return 27.728 * pow(map(analogRead(irR3), 0, 1023, 0, 5000) / 1000.0, -1.2045) -2.2;
}//5,15,24,31

// FRONT RIGHT
float readIR4() {
  return 27.728 * pow(map(analogRead(irR4), 0, 1023, 0, 5000) / 1000.0, -1.2045) -6.5;
}//5,15,25,29

// LEFT
float readIR5() {
  return 27.728 * pow(map(analogRead(irR5), 0, 1023, 0, 5000) / 1000.0, -1.2045) -5.5;
}//5,12,23,30

// RIGHT
float readIR6() {
  return 27.728 * pow(map(analogRead(irR6), 0, 1023, 0, 5000) / 1000.0, -1.2045) -6.5;
}//5,10,20,30

void checkRawValues (){
 float tmp = readIR1();
 Serial.println("obs:"+String(tmp)+"|"+String(estLong(tmp))
                     //+String(readIR2())+"|"
                     //+String(readIR3())+"|"
                     //+String(readIR4())+"| avg: "
                     //+String((readIR2() + readIR3() + readIR4()) / 3.)
                    //  +String(readIR5())+"|"
                    //  +String(readIR6())
                );
        // Serial.println("obs:"+String(median(irArr1,irsampleSize))+"|"
        //               +String(median(irArr2,irsampleSize))+"|"
        //               +String(median(irArr3,irsampleSize))+"|"
        //               +String(median(irArr4,irsampleSize))+"|"
        //               +String(median(irArr5,irsampleSize))+"|"
        //               +String(median(irArr6,irsampleSize)));
  }

//collects samples for ir Array
void irSamples(int maxnum) {
  for (int i = 0; i < maxnum; i++) {
    irArr1[i] = readIR1();
    irArr2[i] = readIR2();
    irArr3[i] = readIR3();
    irArr4[i] = readIR4();
    irArr5[i] = readIR5();
    irArr6[i] = readIR6();
    delay(2);
  }
}

float median(float nums[], int n) {
  int k = n % 2 == 0 ? n / 2 : n / 2 + 1;
  return qselect(nums, 0, n - 1, k - 1);
}

float qselect(float A[], int start, int end, int k) {
  if (start == end) {
    return A[start];
  }

  int left = start, right = end;
  int index = (left + right) / 2;
  float pivot = A[index];

  while (left <= right) {
    while (left <= right && A[left] > pivot) {
      left++;
    }

    while (left <= right && A[right] < pivot) {
      right--;
    }

    if (left <= right) {
      float tmp = A[left];
      A[left] = A[right];
      A[right] = tmp;

      left++;
      right--;
    }
  }

  if (k >= start && k <= right) {
    return qselect(A, start, right, k);
  }
  if (k >= left && k <= end) {
    return qselect(A, left, end, k);
  }
  return A[right + 1];
}


void sendIRtoPC() {
  irSamples(irsampleSize);

  // filtered reading to return in grids
  //automated front calibration
//  if((estShort(median(irArr2, 50))<4 && estShortFR(median(irArr4, irsampleSize))<4) 
//    ||(estShort(median(irArr2, 50))<4 && estShortFM(median(irArr3, irsampleSize))<4)
//    ||(estShortFM(median(irArr3, 50))<4 && estShortFR(median(irArr4, irsampleSize)))<4) {
//       caliFront();
//    }


// cali distance one block
float block_dis = 9.5;

// front calidistance to any 1 block in range
//if(median(irArr2, irsampleSize)<block_dis || 
//   median(irArr3, irsampleSize)<block_dis || 
//   median(irArr4, irsampleSize)<block_dis )caliDistance();

// cali right with various blocks within range 70% reliability
//if(
//  estShort(median(irArr6, irsampleSize)) == 1 &&
//  (median(irArr5, irsampleSize)) == 17.5
//  ) caliRight();

  Serial.println("obs:" + String(estLong(median(irArr1, irsampleSize))) + "|"
                 + String(estShortFL(median(irArr2, irsampleSize))) + "|"
                 + String(estShortFM(median(irArr3, irsampleSize))) + "|"
                 + String(estShortFR(median(irArr4, irsampleSize))) + "|"
                 + String(estShortLeft(median(irArr5, irsampleSize))) + "|"
                 + String(estShortRight(median(irArr6, irsampleSize)))
                );
}

int estShortFL(float reading) {
  if (reading < 9.5) return 0;
  if (reading < 20.5) return 1;
  if (reading <= 33) return 2; 
  return 3;  // 3 grids free or above
}

int estShortFM(float reading) {
  if (reading < 9.5) return 0;
  if (reading < 20.5) return 1;
  if (reading <= 33) return 2; 
  return 3;  // 3 grids free or above
}

int estShortFR(float reading) {
  if (reading < 9.5) return 0;
  if (reading < 21.5) return 1;
  if (reading <= 33) return 2;
  return 3;  // 3 grids free or above
}

int estShortLeft(float reading) {
  if (reading < 9.5) return 0;
  if (reading < 21.3) return 1;
  if (reading <= 34) return 2; 
  return 3;  // 3 grids free or above
}

int estShortRight(float reading) {
  if (reading < 9.5) return 0;
  if (reading < 21.5) return 1;
  if (reading <= 33.5) return 2; 
  return 3;  // 3 grids free or above
}

int estLong(float reading) {
  // if (reading < 24.4) return 0;
  if (reading < 26) return 1;
  if (reading < 33.5) return 2;
  if (reading < 43.5) return 3;
  if (reading < 54) return 4;
  return 5;  // 5 grids or more
}
