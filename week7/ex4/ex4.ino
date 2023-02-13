#include <LiquidCrystal.h>

const int buttonPin = 2; // Pin connected to the button
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32; // LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Initialize the LCD

volatile unsigned long lastTimePressed = 0;
volatile double frequency = 0;

void buttonISR() {
  int currentTime = millis();

  if (lastTimePressed) {
    unsigned long diff = currentTime - lastTimePressed;
    frequency = 1000.0 / diff;
  }

  lastTimePressed = currentTime;
}

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, RISING);
}

void loop() {
  delay(100);
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("Frequency:");  
  lcd.setCursor(0,1);
  lcd.print(frequency);
}