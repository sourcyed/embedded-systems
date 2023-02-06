/* code edited on 30.1. by Enes Doganay and Josef Polasek */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float blinkingFrequency;
int pinState;

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
  const int pulse_high = 200;
  const int pulse_low = 50;
  const int pulse_duration = pulse_high + pulse_low;
  digitalWrite(13, 1);  // turn the LED on (HIGH is the voltage level)
  delay(pulse_high);          // wait for a second
  digitalWrite(13, 0);  // turn the LED off by making the voltage LOW
  delay(pulse_low);           // wait for a second

  if (analogRead(A2) < 200) {
    lcd.setCursor(0, 0);
    lcd.print("Warning!");
    lcd.setCursor(0, 1);
    lcd.print("Value too low!");
  }
  else {
    int pinState = digitalRead(6);

    lcd.setCursor(0, 0);
    lcd.print("Dig." + String(pinState));

    lcd.setCursor(7, 0);
    lcd.print("Ana.:" + String(analogRead(A2)));

    calculate(pulse_duration, pulse_high);
  }
}

void calculate(int pulse_duration, int high_duration) {
  float freq, duty_cycle;
  freq = 1 / (float)pulse_duration;
  duty_cycle = ((float)high_duration / (float)pulse_duration) * 100;
  lcd.setCursor(0, 1);
  lcd.print(String("dut:") + String((int)duty_cycle) + String("%"));
  lcd.setCursor(7, 1);
  lcd.print(String("fr:") + String(freq) + String("Hz"));
}
