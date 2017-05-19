#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

//initializes INA219 modues and their i2c addresses
Adafruit_INA219 ina219_A(0x40);
Adafruit_INA219 ina219_B(0x41);
Adafruit_INA219 ina219_C(0x42);
Adafruit_INA219 ina219_D(0x43);
Adafruit_INA219 ina219_E(0x44);
Adafruit_INA219 ina219_F(0x45);
Adafruit_INA219 ina219_G(0x46);
Adafruit_INA219 ina219_H(0x47);

//sets number of boards in current setup and the number of charging modules per board
const byte numBoards = 2;
const byte chargersperBoard = 8;
//sets minimum voltage (mV) that cells can reach during discharge
const float minV = 3.3;
//sets variable names for shift register pins
const byte RSER = 10;
const byte SRCLK = 11;
const byte RLATCH = 12;
//Names pushbutton pins
const int sel = A1;
const int fw = A2;
const int bk = A0;
//creates an array of bytes, each of which creates a different charge/discharge arrangement when
//shifted into the shift registers
byte registerBytes[9] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x0F, 0x20, 0x40, 0x80};
//a friendlier decimal record of which module on each board is discharging
unsigned short discharging[numBoards];
//tracks the capacity of each battery during and after discharge
float batCap[numBoards][chargersperBoard];
//creates variables used in recording elapsed time between voltage & current readings. Used in battery capacity summations.
unsigned long prevTime;
//creates arrays that store the most recent current and voltage readings from each INA219 sensor
float vBat[numBoards];
float iReading[numBoards];
//tracks whether the system as a whole is in datalogging mode or simply charging all batteries. (0 = charging. 1 = discharging)
char mode = 0;
//tracks whether the system is tethered and using the serial monitor or untethered and using the LCD & pushbutton interface (0 = LCD. 1 = Serial)
char displayMode = 0;

//defines pins used for the LCD
LiquidCrystal lcd(7, 6, 2, 3, 4, 5);


void setup() {
  //initializes sensors
  initSensors();
  
  //Sets shift register pins as outputs
  initRegisters();
  
  //Sets UI button pins as inputs & enables corresponding internal pullup resistors
  initButtons();
  
  //begins charging all batteries
  chargeAll();
  chargeAll();
  
  //initializes the LCD display (16x2 by default)
  //initLCD();
  
  //initializes the serial console
  initSerial();
  
  //takes user input to determine whether to charge or discharge batteries
  getmodeInput();
}

void loop() {
  if(mode == 1){
    //reads voltage and current from each INA219, and updates capacity measurements accordingly
    getV();
    getI();
    logPower();

    //displays information on the progress of the discharge
    displayProgress();
    
    //Moves on to the next battery if the one currently discharging has reached the minimum voltage
    checkMinV();

    delay(1000);

    //switches to charging mode if all batteries are done discharging
    mode = 0;
    for(int i = 0; i < numBoards; i++){
      if(discharging[i]){
        mode = 1;
      }
    }
    
    //if the mode has changed to charging mode, charge all batteries and display collected data
    if(!mode){
      chargeAll();
      outputData();
    }
  }
}

//switches to the next battery or ends the charging cycle if
//a discharging battery's voltage falls below minV
void checkMinV(){
  for(int i = 0; i < numBoards; i++){
    if(vBat[i] < minV && discharging[i]){
      discharging[i] ++;
      if(discharging[i] >= chargersperBoard){
        discharging[i] = 0;
      }
      regUpdate();
    }
  }
}

//Initializes shift register control pins as outputs
void initRegisters(){
  pinMode(RSER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RLATCH, OUTPUT);
}

//intiializes UI button pins as inputs & enables corresponding pullups
void initButtons(){
  pinMode(sel, INPUT_PULLUP);
  pinMode(fw, INPUT_PULLUP);
  pinMode(bk, INPUT_PULLUP);
}

