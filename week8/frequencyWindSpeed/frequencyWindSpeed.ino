#include <LiquidCrystal.h>
#include <Keypad.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonPin = 22; // Pin connected to the button
volatile unsigned long lastTimePressed = 0;
volatile double frequency = 0;

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

volatile bool isWindSpeed = true;
const int frequencyInput = 23;

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
  isWindSpeed = !isWindSpeed;
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

  char key = customKeypad.getKey();

  if (key){
    Serial.print(key);
    if (key == '*')
    isWindSpeed = !isWindSpeed;
  }

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
