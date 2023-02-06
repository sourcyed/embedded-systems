#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float blinkingFrequency;
int pinState;

void setup() {
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, INPUT);
  pinMode(A2, INPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  lcd.clear();
  float pin = analogRead(A2);
  float voltage = (5*(pin/1023));
  if (voltage <= 0.5){
    lcd.print(50);
    digitalWrite(10,0);
  }
  else if ((voltage <= 1.5) && (voltage > 0.5)){
    lcd.print(100);
    digitalWrite(10,0);
  }
  else if ((voltage > 1.5) && (voltage <= 2.5)){
      lcd.print(150);
      digitalWrite(10,0);
    }  
  else if ((voltage > 2.5) && (voltage <= 3.5)){
    lcd.print(200);
    digitalWrite(10,0);
  }
  else if ((voltage > 3.5) && (voltage <= 4.5)){
    lcd.print(250);
    digitalWrite(10, 1);
  }
  else if ((voltage > 4.5) && (voltage <= 5)){
    lcd.print(300);
    digitalWrite(10, 1);
  }
  else {
    lcd.print("Error");
  }
}
