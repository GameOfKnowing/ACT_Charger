//Basic functions to call either the display or serial interface functions depending on whether the device is tethered.

//puts unit into charge or discharge mode based on user input
void getmodeInput(){
  if(displayMode){
    getmodeSerial();
  }
  else{
    getmodeLCD();
  }
}

void displayProgress(){
  if(displayMode){
    progressSerial();
  }
  else{
    progressLCD();
  }
}

void outputData(){
  if(displayMode){
    dataSerial();
  }
  else{
    dataLCD();
  }
}

