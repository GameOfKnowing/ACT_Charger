//Functions for communication with a serial monitor. 


//Initializes the serial console and displays a splashscreen 
void initSerial(){
  Serial.begin(9600);
  Serial.println("ACT LiPo Charger");
  Serial.println("Firmware V1.3");
  displayMode = 1;
}


//takes input from the serial monitor when the unit is first powered on and begins either discharging or charging
void getmodeSerial(){
  while(!Serial){}
  Serial.println("Charge or discharge? (c/d)");
  char charRecieved = 0;
  //waits for user input to serial console
  while(!charRecieved){
    if(Serial.available()) {
      char ch = Serial.read();
      Serial.println(ch);
      //if user enters "c", continues in charging mode
      if(ch == 'c'){
        chargeAll();
        Serial.println("charging!");
        charRecieved++;
      }
      //if user inputs "d", begins discharging mode
      else if(ch == 'd'){
        beginDischarge();
        mode = 1;
        Serial.println("discharging!");
        charRecieved++;
      }
    }
  }
}

void progressSerial(){
  Serial.println("Discharging... Progress:");
  for(int i = 0; i < numBoards; i++){
    Serial.print("Board ");
    Serial.print(i);
    Serial.print(": Discharging Battery # ");
    Serial.print(discharging[i]);
    Serial.println(".");
    Serial.print("Voltage: ");
    Serial.print(vBat[i]);
    Serial.print("V. Current: ");
    Serial.print(iReading[i]);
    Serial.println("mA.\n");
  }
}

void dataSerial(){
  Serial.println("Discharging complete! Data collected:");
  for(int i = 0; i < numBoards; i++){
    Serial.print("Board # ");
    Serial.println(i);
    for(int j = 0; j < chargersperBoard; j++){
      Serial.print("Battery ");
      Serial.print(j);
      Serial.print(" capacity: ");
      Serial.print(batCap[i][j]);
      Serial.println(" mWh.");
    }
    Serial.println("\n");
  }
}

