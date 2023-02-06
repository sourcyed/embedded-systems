#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float blinkingFrequency;
int pinState;

int AD_conversion(byte ch)
{
  DIDR0=B11111111;

  ADMUX=B01000000 | ch;

  ADCSRA=B11000111;

  while( (ADCSRA & B01000000) != 0);

  return ADC;
}

void setup() {
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, INPUT);
  pinMode(A2, INPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);

  float time1, time2, time3;

  time1 = micros();
  analogRead(A2);  
  time2 = micros() - time1;
  AD_conversion(A2);
  time3 = micros() - time1;

  lcd.print(String("Ana.:") + String(time2));
  lcd.setCursor(0,1);
  lcd.print(String("Reg.:") + String(time3));
}

void loop() {
  // put your main code here, to run repeatedly:
  // delay(1000);
  // lcd.clear();
  // float pin = analogRead(A2);
  // float voltage = (5*(pin/1023));
  // if (voltage <= 0.5){
  //   lcd.print(50);
  //   digitalWrite(10,0);
  // }
  // else if ((voltage <= 1.5) && (voltage > 0.5)){
  //   lcd.print(100);
  //   digitalWrite(10,0);
  // }
  // else if ((voltage > 1.5) && (voltage <= 2.5)){
  //     lcd.print(150);
  //     digitalWrite(10,0);
  //   }  
  // else if ((voltage > 2.5) && (voltage <= 3.5)){
  //   lcd.print(200);
  //   digitalWrite(10,0);
  // }
  // else if ((voltage > 3.5) && (voltage <= 4.5)){
  //   lcd.print(250);
  //   digitalWrite(10, 1);
  // }
  // else if ((voltage > 4.5) && (voltage <= 5)){
  //   lcd.print(300);
  //   digitalWrite(10, 1);
  // }
  // else {
  //   lcd.print("Error");
  // }
}
