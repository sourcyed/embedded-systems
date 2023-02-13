/* code edited on 30.1. by Enes Doganay and Josef Polasek */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int pinState;
int i = 0;
int c = 0;
int lineLength = 32;
int startingAscii = 65;
int endingAscii = 90;

// the setup function runs once when you press reset or power the board
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, INPUT);
  pinMode(A2, INPUT);
  pinMode(13, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  delay(500);
  lcd.clear();
  calculate();
}

void calculate() {
  i %= lineLength;
  c %= endingAscii - startingAscii;
  
  int x, y;
  if (i < lineLength) {
    x = i;
    y = 0;
  }
  else {
    x = (lineLength - 1) - (i - lineLength);
    y = 1;
  }
  lcd.setCursor(x, y);
  lcd.print((char)(c + startingAscii));

  i++;
  c++;
}
