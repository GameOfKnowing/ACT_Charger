//Functions to control the shift registers

//Output: Low = charge; High = discharge
//Fills all registers with 0's and shifts out, charging all batteries
void chargeAll(){
  //sets all shift register pins low, charging all batteries
  digitalWrite(RLATCH, LOW);
  for(byte i = 0; i < 8; i++){
    shiftOut(RSER, SRCLK, MSBFIRST, registerBytes[0]);
  }
  digitalWrite(RLATCH, HIGH);

  //stores 0's to all boards' variables in the "discharing" array, indicating that no boards
  //are discharging
  for(byte i = 0; i < numBoards; i++){
      discharging[i] = 0;
  }
}

//Discharges the first battery of each board, beginning discharge process
bool beginDischarge(){
  digitalWrite(RLATCH, LOW);
  for(byte i = 0; i < numBoards; i++){
    shiftOut(RSER, SRCLK, MSBFIRST, registerBytes[1]);
  }
  digitalWrite(RLATCH, HIGH);
  //sets discharging array to initial conditions (1st battery on each board discharging)
  for(byte i; i < numBoards; i++){
    discharging[i] = 1 ;
  }
  return true;
}

//Updates shift registers to reflect batter states indicated by batState array
void regUpdate(){
  digitalWrite(RLATCH, LOW);
  for(short i = numBoards - 1; i >= 0; i--){
    byte registerByte = registerBytes[discharging[i]];
    shiftOut(RSER, SRCLK, MSBFIRST, registerByte);
  }
  digitalWrite(RLATCH, HIGH);
}
