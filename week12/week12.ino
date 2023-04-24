#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <TimerOne.h>
#include <Keypad.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //set up lcd 
////////////////////////////////////////////////////////////////////////////////////////////////////////
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

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), pin_rows, pin_column, ROWS, COLS); //set up keypad
////////////////////////////////////////////////////////////////////////////////////////////////////////
EthernetClient ethClient; //Create the eth object
static uint8_t mymac[6] = { 0x44,0x76,0x58,0x10,0x00,0x62 };
<<<<<<< HEAD

//  MQTT settings
 
unsigned int Port = 1883;                          //  MQTT port number
byte server[] = { 10,6,0,21 };    
=======
unsigned int Port = 1883;                         
byte server[] = { 10,6,0,20 }; 
>>>>>>> be0451f0f85192191b52e11bfa4bcf5c62c1769c

char* deviceId = "picha"; //
char* clientId = "pi2023";
char* deviceSecret = "tamk";

void callback(char* topic, byte* payload, unsigned int lenght);
PubSubClient client(server, Port, callback, ethClient); 
                                                                                           //
#define inTopic    "myTopic"                    // MQTT channel where data are received   //
#define outTopic   "myTopic"                                                             //
//////////////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
const int frequencyInput = 23;
=======
#define inTopic    "ICT18_in_2020"                    // MQTT channel where data are received 
#define outTopic   "ICT18_out_2020" 
>>>>>>> be0451f0f85192191b52e11bfa4bcf5c62c1769c

void waveformISR() {
    
}

float windSpeed = 0;
void setup() {
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial.println("Start 20.3.2023");
  //pinMode(A2, INPUT);
  fetch_IP();
  Connect_MQTT_server();  
}

void loop() {
  delay(400);
  lcd.clear();
  //float pin = analogRead(A2);
  //float voltage = (5*(pin/1023));
  int inx=10;
  while(true){
    lcd.clear();
    Serial.println("Are we connected?");
    lcd.println("Are we connected?");
    send_MQTT_message(inx);
    inx++;
    delay(1500);
  }
 
}

void fetch_IP(void){
  byte rev=1;
  lcd.setCursor(0,1); 
  lcd.print("     Waiting IP     ");
  rev=Ethernet.begin( mymac);                  // get IP number
  Serial.print( F("\nW5100 Revision "));
  lcd.print( F("\nW5100 Revision "));
  if (rev==0){
    Serial.println( F("Failed to access Ethernet controller"));
  }

  Serial.println( F("Setting up DHCP"));
  lcd.print("Connected with IP: ");
  lcd.print(Ethernet.localIP());
  Serial.print("Connected with IP: "); 
  Serial.println(Ethernet.localIP()); 
  delay(1500);  
  lcd.clear();
}

void send_MQTT_message(int num){                     // Send MQTT message
  char bufa[50];                             //  Print message to serial monitor
  if (client.connected()){ 
    sprintf(bufa,"My_MQTT_message: value =%d", num);               // create message with header and data
    Serial.println( bufa ); 
    client.publish(outTopic,bufa);                        // send message to MQTT server        
  }
  else{                                                           //   Re connect if connection is lost
    delay(500);
    Serial.println("No, re-connecting" );
    lcd.clear();
    lcd.print("No, re-conneting");
    client.connect(clientId, deviceId, deviceSecret);
    delay(1000);                                            // wait for reconnecting
  }
}

//  MQTT server connection  
                                           
void Connect_MQTT_server(){ 
  Serial.println(" Connecting to MQTT" );
  Serial.print(server[0]); Serial.print(".");     // Print MQTT server IP number to Serial monitor
  Serial.print(server[1]); Serial.print(".");
  Serial.print(server[2]); Serial.print(".");
  Serial.println(server[3]); 
  delay(500);
  
  if (!client.connected()){                                   // check if allready connected  
    if (client.connect(clientId, deviceId, deviceSecret)){ // connection to MQTT server 
      Serial.println(" Connected OK " );
      client.subscribe(inTopic);                        // subscript to in topic        
    } 
    else{
       Serial.println(client.state());
    }    
  }
}

//  Receive incoming MQTT message   
 
void callback(char* topic, byte* payload, unsigned int length){ 
  char* receiv_string;                               // copy the payload content into a char* 
  receiv_string = (char*) malloc(length + 1); 
  memcpy(receiv_string, payload, length);           // copy received message to receiv_string 
  receiv_string[length] = '\0';           
  Serial.println( receiv_string );
  free(receiv_string); 
} 
