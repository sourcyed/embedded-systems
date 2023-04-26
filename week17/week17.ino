/**
 * @file week17.ino
 * @authors Josef Polasek, Enes Doganay, Sylvi Kokko
 * @brief Project for Embedded Systems class (TAMK, Spring 2023)
 * @date 2023-04-24
 */

/* LIBRARIES */
#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <TimerOne.h>
#include <Keypad.h>

/* VARIABLES */
// LCD
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Button
int buttonPin = 10;

// Keypad
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte pin_rows[ROWS] = { 2, 3, 4, 5 };
byte pin_column[COLS] = { 6, 7, 8, 9 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), pin_rows, pin_column, ROWS, COLS);

// Ethernet
EthernetClient ethClient;
byte server[] = { 10, 6, 0, 21 };  // TAMK IP
#define mac_6 0x73
static uint8_t mymac[6] = { 0x44, 0x76, 0x58, 0x10, 0x00, mac_6 };

// Wind Speed
volatile float time = 1;
float speedArray[10];
int sdCount = 0;
float speedAvg = 0;

// Wind Direction
float dirArray[10];
float dirAvg = 0;

void fetchIP(void);
#define mac_6 0x73
byte rev = 1;
char outstrg[4];

void fetchIP() {

  lcd.setCursor(0, 1);
  lcd.print("     Waiting IP     ");
  rev = Ethernet.begin(mymac);  // get IP number

  Serial.print(F("\nW5100 Revision "));

  if (rev == 0) {
    Serial.println(F("Failed to access Ethernet controller"));
    lcd.setCursor(0, 0);
    lcd.print(" Ethernet failed   ");
  }
  Serial.println(F("Setting up DHCP"));
  Serial.print("Connected with IP: ");
  Serial.println(Ethernet.localIP());
  lcd.setCursor(0, 1);
  lcd.print("                     ");
  lcd.setCursor(0, 1);
  lcd.print("myIP=");
  lcd.print(Ethernet.localIP());
  delay(1500);
}

unsigned int Port = 1883;     //  MQTT port number
char *deviceId = "picha";     // * set your device id (will be the MQTT client username) *yksilöllinen*
char *clientId = "likepi";    // * set a random string (max 23 chars, will be the MQTT client id) *yksilöllinen*
char *deviceSecret = "tamk";  // * set your device secret (will be the MQTT client password) *kaikille yhteinen*

//  MQTT Server settings

void callback(char *topic, byte *payload, unsigned int length);  // subscription callback for received MQTTT messages

PubSubClient client(server, Port, callback, ethClient);  // mqtt client

//  MQTT topic names

#define inTopic "ICT1B_in_2020"   // * MQTT channel where data are received
#define outTopic "ICT4_out_2020"  // * MQTT channel where data is send

/* setup  */
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, INPUT);
  pinMode(A2, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(13, OUTPUT);
  fetchIP();              // initialize Ethernet connection
  Connect_MQTT_server();  // connect to MQTT server

  /* count for windSpeed */
  attachInterrupt(digitalPinToInterrupt(11), count, RISING);
}

void Connect_MQTT_server() {
  Serial.println(" Connecting to MQTT");
  Serial.print(server[0]);
  Serial.print(".");  // Print MQTT server IP number to Serial monitor
  Serial.print(server[1]);
  Serial.print(".");
  Serial.print(server[2]);
  Serial.print(".");
  Serial.println(server[3]);
  delay(500);

  if (!client.connected()) {                                 // check if allready connected
    if (client.connect(clientId, deviceId, deviceSecret)) {  // connection to MQTT server
      Serial.println(" Connected OK ");
      client.subscribe(inTopic);  // subscript to in topic
    } else {
      Serial.println(client.state());
    }
  }
}
void send_MQTT_message(int index, float num1, float num2) {  // Send MQTT message
  char bufa[50];                                             //  Print message to serial monitor
  if (client.connected()) {
    if (index == 1) {
      dtostrf(num1, 4, 1, outstrg);
      sprintf(bufa, "IOTJS={\"S_name1\":\"-speed-\",\"S_value1\":%s}", outstrg);
    }  // create message with header and data
    else if (index == 2) {
      dtostrf(num1, 4, 1, outstrg);
      sprintf(bufa, "IOTJS={\"S_name1\":\"-direction-\",\"S_value1\":%s}", outstrg);  // create message with header and data
    } else if (index == 3) {
      dtostrf(num1, 4, 1, outstrg);
      sprintf(bufa, "IOTJS={\"S_name1\":\"-speed-\",\"S_value1\":%s}", outstrg);
      dtostrf(num2, 4, 1, outstrg);
      sprintf(bufa, "IOTJS={\"S_name2\":\"-direction-\",\"S_value2\":%s}", outstrg);
    }
    Serial.println(bufa);
    client.publish(outTopic, bufa);
    Serial.println("Message was sent");
  } else {  //   Re connect if connection is lost
    delay(500);
    Serial.println("No, re-connecting");
    client.connect(clientId, deviceId, deviceSecret);
    delay(1000);  // wait for reconnecting
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  char *receiv_string;  // copy the payload content into a char*
  receiv_string = (char *)malloc(length + 1);
  memcpy(receiv_string, payload, length);  // copy received message to receiv_string
  receiv_string[length] = '\0';
  Serial.println(receiv_string);
  free(receiv_string);
}

float getDegrees(float voltage) {
  return voltage * 360 / 5;
}

/* This is for time for the windSpeed */

int pressed = 0;
float startTime, endTime;

void count() {
  if (pressed == 0) {
    startTime = millis();
  } else if (pressed == 1) {
    endTime = millis();
    time = endTime - startTime;
  } else {
    startTime = endTime;
    endTime = millis();
    time = endTime - startTime;
  }
  pressed++;
}

volatile int printWhich = 1;  // weird way to do this but it works

/* main code that's running repeatedly */
void loop() {
  delay(100);
  lcd.clear();

  float dirPin = analogRead(A3);

  // getting values every second
  float dir = getDegrees(5 * (dirPin / 1023));
  float speed = -0.24 + (1 / (time / 1000)) * 0.699;

  speedArray[sdCount % 10] = speed;
  dirArray[sdCount % 10] = dir;
  sdCount++;

  // calculate speed and direction averages

  if (sdCount > 10)  // we have at least 10 measurements --> creating average, otherwise 0
  {
    float speedSum = 0;
    for (int i = 0; i < 10; i++) {
      speedSum += speedArray[i];
    }

    float dirSum = 0;
    for (int i = 0; i < 10; i++) {
      dirSum += dirArray[i];
    }

    speedAvg = speedSum / 10;
    dirAvg = dirSum / 10;
  }

  lcd.setCursor(0, 0);

  char key = customKeypad.getKey();

  if (key) {
    Serial.print(key);

    if (key == '*') {
      if (printWhich != 3)
        printWhich += 1;
      else if (printWhich >= 3)
        printWhich = 1;
    } else if (key == 'D') {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("myIP=");
      lcd.print(Ethernet.localIP());
      delay(1500);
    } else if (key == '#') {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Sending message to server");
      if (printWhich == 1)
        send_MQTT_message(1, speedAvg, 0);
      else if (printWhich == 2)
        send_MQTT_message(2, dirAvg, 0);
      else if (printWhich == 3)
        send_MQTT_message(3, speedAvg, dirAvg);
    } else if (key == '0')
      speed = 0;
    else if (key == '1')
      speed = 1;
    else if (key == '2')
      speed = 2;
    else if (key == '3')
      speed = 3;
    else if (key == '4')
      speed = 4;
    else if (key == '5')
      speed = 5;
    else if (key == '6')
      speed = 6;
    else if (key == '7')
      speed = 7;
    else if (key == '8')
      speed = 8;
    else if (key == '9')
      speed = 9;
  }

  lcd.setCursor(0, 1);
  lcd.print("* change # send");
  lcd.setCursor(0, 0);

  if (printWhich == 1) {
    lcd.print("Speed:");
    lcd.setCursor(7, 0);

    lcd.print(speedAvg);
  } else if (printWhich == 2) {
    lcd.print("Dir:");
    lcd.setCursor(5, 0);
    lcd.print(dirAvg);
  } else if (printWhich == 3) {
    lcd.print(speedAvg);
    lcd.setCursor(8, 0);
    lcd.print(dirAvg);
  }
}