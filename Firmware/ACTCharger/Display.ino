/*code to control a standar 16x2 character LCD display
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 6
 * LCD D4 pin to digital pin 2
 * LCD D5 pin to digital pin 3
 * LCD D6 pin to digital pin 4
 * LCD D7 pin to digital pin 5
 * 
 */

//initializes the LCD and displays a startup screen
void initLCD(){
  // initialize the library with the numbers of the interface pins
  lcd.begin(16, 2);
  pinMode(bk, INPUT_PULLUP);
  pinMode(sel, INPUT_PULLUP);
  pinMode(fw, INPUT_PULLUP);
  lcd.setCursor(0,0);
  lcd.print("ACT LiPo Charger");
  lcd.setCursor(0,1);
  lcd.print("Firmware V1.0");
  lcd.noCursor();
  delay(2000);
  lcd.clear();
}

//displays a user prompt on the LCD asking whether to charge or discharge, then takes the resulting user input and starts the indicated process
void getmodeLCD(){
  lcd.setCursor(0,0);
  lcd.print("Discharge?");
  lcd.setCursor(7,1);
  lcd.print("Y/N");
  lcd.cursor();
  char hasPicked = 0;
  char selection = 1;
  //polls pushbuttons and updates the display accordingly until the user selects Y/N
  while(!hasPicked){
    char prevSelection = selection;
    if(!digitalRead(bk)){
      selection = 0;
    }
    else if (!digitalRead(sel)){
      hasPicked = 1;
    }
    else if (!digitalRead(fw)){
      selection = 1;
    }
    if(prevSelection != selection){
      if(selection){
        lcd.setCursor(10,1);
      }
      else{
        lcd.setCursor(6,0);
      }
    }
  }
  
  cleanSlate();
  //sets global charge/discharge mode based on selection
  if(selection){
    chargeAll();
    mode = 0;
    lcd.print("Charging!");
    delay(1000);
  }
  else{
    beginDischarge();
    mode = 1;
    lcd.print("Discharging!");
    delay(1000);
  }
}

//displays the number of the unit currently being discharged on each board
void progressLCD(){
  cleanSlate();
  lcd.print("B0 1 2 3 4 5 6 7");
  lcd.setCursor(0,1);
  lcd.print("U");
  for(char i = 0; i < numBoards; i++){
    lcd.print(discharging[i]);
    lcd.print(" ");
  }
}

//displays a multi-page list of a splash-screen, a list of boards with potentially problematic batteries, & battery capacities,
//polling user button presses to navigate the list.
void dataLCD(){
  char page = 0;
  char updateDisplay = 1;
  while(1){
    if(!digitalRead(bk) && (page >= 1)){
      page--;
      updateDisplay = 1;
    }
    else if(!digitalRead(fw) && (page < (2+(numBoards*chargersperBoard)))){
      page++;
      updateDisplay = 1;
    }
    if(updateDisplay){
      updateDisplay = 0;
      cleanSlate();
      if(page == 0){
        lcd.print("Discharge Done!");
        lcd.setCursor(0,1);
        lcd.print("Scroll for info.");
      }
      else if(page == 1){
        lcd.print("Check Boards:");
        lcd.setCursor(0,1);
        lcd.print(checkboardString());
      }
      else {
        char thisBoard = (page-2)/chargersperBoard;
        char thisUnit = (page-2)%chargersperBoard;
        lcd.print("B");
        lcd.print(thisBoard);
        lcd.print("U");
        lcd.print(thisUnit);
        lcd.print(" Cap. (mWh):");
        lcd.setCursor(0,1);
        lcd.print(batCap[thisBoard][thisUnit]);
      } 
      delay(50);
    }
  }
}

//creates a list for LCD data output of boards with under-performing batteries based on capacity measurements
String checkboardString(){
    float avgCap;
    char problem = 0;
    String output = "";
    for(char i = 0; i < numBoards; i++){
      for(char j = 0; j < chargersperBoard; j++){
        avgCap += batCap[i][j];
      }
    }
    avgCap = (avgCap/(numBoards*chargersperBoard));
    for(char i = 0; i < numBoards; i++){
      for(char j = 0; j < chargersperBoard; j++){
        float deviation = batCap[i][j] - avgCap;
        if(deviation > 100.0 || deviation < -100.0){
          problem = 1;
        }
      }
      if(problem){
      output += i;
      output += " ";
      problem = 0;
      }
    }
    return output;
}

//clears the display, sets the cursor to its home position, and makes the cursor invisible
void cleanSlate(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.noCursor();
}

