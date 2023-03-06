#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonPin = 10; // Pin connected to the button
volatile bool printVoltage = true;



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
  lcd.setCursor(0, 0);
  if (printVoltage) {
    lcd.print(voltage);
  }
  else
  {
    if ((voltage <= 1.4)){
      lcd.print("North");
    }
    else if ((voltage <= 2.00) && (voltage > 1.4)){
      lcd.print("North East");     
    }
    else if ((voltage > 2.0) && (voltage <= 2.8)){
        lcd.print("East");
      }  
    else if ((voltage > 2.8) && (voltage <= 3)){
      lcd.print("South East");
    }
    else if ((voltage > 3.0) && (voltage <= 3.2)){
      lcd.print("South");
    }
    else if ((voltage > 3.2) && (voltage <= 3.9)){
      lcd.print("South West");
    }
    else if ((voltage > 3.9) && (voltage <= 4.3)){
      lcd.print("West");
    }
      else if ((voltage > 4.3) && (voltage < 4.8)){
          lcd.print("North West");
        }   
    else {
      lcd.print("North");
    }
  }
}

