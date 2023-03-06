#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonPin = 2; // Pin connected to the button
volatile unsigned long lastTimePressed = 0;
volatile double frequency = 0;

volatile bool isWindSpeed = true;

const int frequencyInput = 3;

float windSpeed = 0;

void waveformISR() {
      int currentTime = millis();
    if (lastTimePressed) {
      unsigned long diff = currentTime - lastTimePressed;
      frequency = 1000.0 / diff;
    }
    lastTimePressed = currentTime;
}

void buttonISR () {
  if (isWindSpeed == true) isWindSpeed = false;
  else isWindSpeed = true;
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(frequencyInput, INPUT);
  attachInterrupt(digitalPinToInterrupt(frequencyInput), waveformISR, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, RISING);
}

void loop() {
  delay(100);
  lcd.clear();
  lcd.setCursor(0,0);

  if (isWindSpeed == true) {
      windSpeed = -0.24 + frequency * 0.699;
      lcd.setCursor(0,0);
      lcd.print("Wind speed:");
      lcd.setCursor(0,1);
      lcd.print(windSpeed);
  } else {
    lcd.setCursor(0,0);
    lcd.print("Frequency:");
    lcd.setCursor(0,1);
    lcd.print(frequency);
  }
}
