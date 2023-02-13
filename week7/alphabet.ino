#include <LiquidCrystal.h>

const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

int i = 65; // until 90
int col = 0;
int row = 0;

void loop() {
  // put your main code here, to run repeatedly:
  delay(400);
  lcd.clear();
  
  lcd.setCursor(col, row);
  
  lcd.print(char(i++));

  if (i > 90) {
    i = 65;
  }
  
  if (row == 0) {
    if (col == 15) {
      row = 1;
    }
    else {
      col += 1;
    }
  } 
  else if (row == 1) {
    if (col == 0) {
      row = 0;
    } else {
      col -= 1;
    }
  }
  
}
