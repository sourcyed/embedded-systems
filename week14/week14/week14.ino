#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <TimerOne.h>
#include <Keypad.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 37, en = 36, d4 = 35, d5 = 34, d6 = 33, d7 = 32;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buttonPin = 10; // Pin connected to the button
volatile bool printVoltage = true;

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

EthernetClient ethClient;                               // Ethernet object var  
void fetch_IP(void);
#define  mac_6    0x73
static uint8_t mymac[6] = { 0x44,0x76,0x58,0x10,0x00,mac_6 };
byte rev=1;

void fetchIP(){

  lcd.setCursor(0,1);  
  lcd.print("     Waiting IP     ");
  rev=Ethernet.begin( mymac);                  // get IP number
     
  Serial.print( F("\nW5100 Revision ") );
    
  if ( rev == 0){          
    Serial.println( F( "Failed to access Ethernet controller" ) );
    lcd.setCursor(0,0); lcd.print(" Ethernet failed   ");
  }           
  Serial.println( F( "Setting up DHCP" ));
  Serial.print("Connected with IP: "); 
  Serial.println(Ethernet.localIP()); 
  lcd.setCursor(0,1);
  lcd.print("                     ");
  lcd.setCursor(0,1);
  lcd.print("myIP=");
  lcd.print(Ethernet.localIP());
  delay(1500);
}
unsigned int Port = 1883;                          //  MQTT port number
byte server[] = { 10,6,0,20 };                    // TAMK IP

char* deviceId     = "picha";                   // * set your device id (will be the MQTT client username) *yksilöllinen*
char* clientId     = "likepi";                       // * set a random string (max 23 chars, will be the MQTT client id) *yksilöllinen*
char* deviceSecret = "tamk";                        // * set your device secret (will be the MQTT client password) *kaikille yhteinen*

//  MQTT Server settings  

void callback(char* topic, byte* payload, unsigned int length); // subscription callback for received MQTTT messages   

PubSubClient client(server, Port, callback, ethClient);   // mqtt client 

//  MQTT topic names 

 #define inTopic    "ICT1B_in_2020"                    // * MQTT channel where data are received 
 #define outTopic   "ICT1B_out_2020"                   // * MQTT channel where data is send 
                          

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

void setup() {
 // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(6, INPUT);
  pinMode(A2, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(13, OUTPUT);
  fetchIP();                                         // initialize Ethernet connection
  Connect_MQTT_server();                              // connect to MQTT server 
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
   lcd.clear();
  float pin = analogRead(A2);
  float voltage = (5*(pin/1023));


  lcd.setCursor(0, 0);

  char key = customKeypad.getKey();

  if (key){
    Serial.print(key);
    if (key == '*'){
      printVoltage = !printVoltage;
    }
    else if (key == 'D'){}
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("myIP=");
      lcd.print(Ethernet.localIP());
      delay(1500);
    }
    else if (key == 'C'){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Sending message to server");
      send_MQTT_message(voltage);
    }

  
  if (printVoltage) {
    int degrees = voltage * 360/5;

    lcd.print(degrees);
  }
  else
  {
    if ((voltage <= 1.4)){
      lcd.print("North");
    }
    else if ((voltage <= 2.00) && (voltage > 1.4)){
      lcd.print("North East");     
    }
    else if ((voltage > 2.0) && (voltage <= 2.8)){
        lcd.print("East");
      }  
    else if ((voltage > 2.8) && (voltage <= 3)){
      lcd.print("South East");
    }
    else if ((voltage > 3.0) && (voltage <= 3.2)){
      lcd.print("South");
    }
    else if ((voltage > 3.2) && (voltage <= 3.9)){
      lcd.print("South West");
    }
    else if ((voltage > 3.9) && (voltage <= 4.3)){
      lcd.print("West");
    }
    else if ((voltage > 4.3) && (voltage < 4.8)){
        lcd.print("North West");
      }   
    else {
      lcd.print("North");
    }
  }
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
    client.connect(clientId, deviceId, deviceSecret);
    delay(1000);                                            // wait for reconnecting
  }
}



void callback(char* topic, byte* payload, unsigned int length){ 
  char* receiv_string;                               // copy the payload content into a char* 
  receiv_string = (char*) malloc(length + 1); 
  memcpy(receiv_string, payload, length);           // copy received message to receiv_string 
  receiv_string[length] = '\0';           
  Serial.println( receiv_string );
  free(receiv_string); 
} 