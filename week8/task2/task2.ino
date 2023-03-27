#include <LiquidCrystal.h>
#include <Keypad.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonPin = 10; // Pin connected to the button
volatile bool printVoltage = true;

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte pin_column[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), pin_rows, pin_column, ROWS, COLS);

void buttonISR () {
  printVoltage = !printVoltage;
}

void setup() {
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, INPUT);
  pinMode(A2, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(13, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
   lcd.clear();
  float pin = analogRead(A2);
  float voltage = (5*(pin/1023));

  char key = customKeypad.getKey();

  if (key){
    Serial.print(key);
    if (key == '*')
      printVoltage = !printVoltage;
  }

  lcd.setCursor(0, 0);
  
  if (printVoltage) {
    lcd.print(voltage);
  }
  else
  {
    if ((voltage <= 1.2 || voltage == 5.0)){
      lcd.print("North");
    }
    else if ((voltage > 1.67) && (voltage <= 2.15)){
      lcd.print("North East");     
    }
    else if ((voltage > 2.15) && (voltage <= 2.63)){
        lcd.print("East");
      }  
    else if ((voltage > 2.63) && (voltage <= 3.10)){
      lcd.print("South East");
    }
    else if ((voltage > 3.10) && (voltage <= 3.58)){
      lcd.print("South");
    }
    else if ((voltage > 3.58) && (voltage <= 4.05)){
      lcd.print("South West");
    }
    else if ((voltage > 4.05) && (voltage <= 4.53)){
      lcd.print("West");
    }
    else {
        lcd.print("North West");
    }   
  }
}

