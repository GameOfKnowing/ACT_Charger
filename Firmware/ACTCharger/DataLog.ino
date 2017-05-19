//Functions to control & communicate with the INA219 sensors

//Begins i2c communication with all INA219 sensors
void initSensors(){
  Serial.print("Initializing sensors...");
  ina219_A.begin();
  if (numBoards > 1){
    ina219_B.begin();
    if (numBoards > 2){
      ina219_C.begin();
      if (numBoards > 3){
        ina219_D.begin();
        if (numBoards > 4){
          ina219_E.begin();
          if (numBoards > 5){
            ina219_F.begin();
            if (numBoards > 6){
              ina219_G.begin();
              if(numBoards > 7){
                ina219_H.begin();
              }
            }
          }
        }
      }
    }
  }
  Serial.println("Sensors initialized.");
}

//updates the logged capacity of each battery under test
void logPower(){
  Serial.print("Logging power...");
  float elapsedTime = getTime();
  for(int i = 0; i < numBoards; i++){
    float milliwattHours = ((vBat[i] * iReading[i] * elapsedTime) / 3600.0);
    char thisBattery = discharging[i] - 1;
    batCap[i][thisBattery] += milliwattHours;
  }
  Serial.println("Power logged.");
}

//Checks voltage readings (in mV) from all boards & loads them into the floating-poing array vBat
//The battery voltage is equal to the sum of the shunt and bus voltages
bool getV(){
  if(numBoards){
    vBat[0] = ina219_A.getShuntVoltage_mV();
    float busVoltage = ina219_A.getBusVoltage_V();
    vBat[0] = busVoltage + (vBat[0]/1000.0);

    if (numBoards > 1){
      vBat[1] = ina219_B.getShuntVoltage_mV();

      //synthesizes the shunt (in mV) and bus (in V) voltages into the battery voltage (V)
      busVoltage = ina219_B.getBusVoltage_V();
      vBat[1] = busVoltage + (vBat[1]/1000.0);

      if (numBoards > 2){
        //synthesizes the shunt (in mV) and bus (in V) voltages into the battery voltage (V)
        vBat[2] = ina219_C.getShuntVoltage_mV();
        busVoltage = ina219_C.getBusVoltage_V();
        vBat[2] = busVoltage + (vBat[2]/1000.0);
        
        if (numBoards > 3){
          vBat[3] = ina219_D.getShuntVoltage_mV();
          
          //synthesizes the shunt (in mV) and bus (in V) voltages into the battery voltage (V)
          busVoltage = ina219_D.getBusVoltage_V();
          vBat[3] = busVoltage + (vBat[3]/1000.0);
          if (numBoards > 4){
            vBat[4] = ina219_E.getShuntVoltage_mV();
            
            //synthesizes the shunt (in mV) and bus (in V) voltages into the battery voltage (V)
            busVoltage = ina219_E.getBusVoltage_V();
            vBat[4] = busVoltage + (vBat[4]/1000.0);
            
            if (numBoards > 5){
              vBat[5] = ina219_F.getShuntVoltage_mV();
              
              //synthesizes the shunt (in mV) and bus (in V) voltages into the battery voltage (V)
              busVoltage = ina219_F.getBusVoltage_V();
              vBat[5] = busVoltage + (vBat[5]/1000.0);
              
              if (numBoards > 6){
                vBat[6] = ina219_G.getShuntVoltage_mV();        
                
                //synthesizes the shunt (in mV) and bus (in V) voltages into the battery voltage (V)
                busVoltage = ina219_G.getBusVoltage_V();
                vBat[6] = busVoltage + (vBat[6]/1000.0);
                
                if(numBoards > 7){
                  vBat[7] = ina219_H.getShuntVoltage_mV();
                  
                  //synthesizes the shunt (in mV) and bus (in V) voltages into the battery voltage (V)
                  busVoltage = ina219_H.getBusVoltage_V();
                  vBat[7] = busVoltage + (vBat[7]/1000.0);
              }
            }
          }
        }
      }
    }
  }
}
  Serial.println("Voltages recorded.");
}

//Checks current readings (in mA) from all boards & loads them into the floating-poing array iReading
bool getI(){
  iReading[0] = ina219_A.getCurrent_mA();
  if (numBoards > 1){
    iReading[1] = ina219_B.getCurrent_mA();
    if (numBoards > 2){
      iReading[2] = ina219_C.getCurrent_mA();
      if (numBoards > 3){
        iReading[3] = ina219_D.getCurrent_mA();
        if (numBoards > 4){
          iReading[4] = ina219_E.getCurrent_mA();
          if (numBoards > 5){
            iReading[5] = ina219_F.getCurrent_mA();
            if (numBoards > 6){
              iReading[6] = ina219_G.getCurrent_mA();
              if(numBoards > 7){
                iReading[7] = ina219_H.getCurrent_mA();
              }
            }
          }
        }
      }
    }
  }
  Serial.println("Currents recorded.");
}

//gets the amount of time since the last reading
float getTime(){
    //takes the time since the last time measurement in milliseconds and converts it to a variable of type float
    //in units of seconds
    float output = ((float)((millis() - prevTime))/1000);
    prevTime = millis();
    return output;
}

