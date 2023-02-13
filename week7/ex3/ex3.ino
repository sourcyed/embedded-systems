#include <LiquidCrystal.h>

const int buttonPin = 2; // Pin connected to the button
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; // LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Initialize the LCD

volatile int buttonCounter = 0; // Counter to store the number of button presses
int i = 65; // until 90
int col = 0;
int row = 0;

void buttonISR() {
  buttonCounter++; // Increment the button counter
  lcd.clear();
  lcd.setCursor(0, 0); // Set the cursor position
  lcd.print("Button presses: ");
  lcd.setCursor(0, 1); // Set the cursor position
  lcd.print(buttonCounter); // Display the button counter value
}

void setup() {
  lcd.begin(16, 2); // Initialize the LCD
  pinMode(buttonPin, INPUT_PULLUP); // Set the button pin as an input with pull-up resistor
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING); // Attach the ISR to the button pin, trigger on a falling edge
}

void loop() {
   // put your main code here, to run repeatedly:
  delay(1000);
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