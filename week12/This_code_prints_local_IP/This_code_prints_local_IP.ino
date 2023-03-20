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

EthernetClient ethClient;                               // Ethernet object var  

void fetch_IP(void);
 
/////////////////////////////////////////////////////////////////////////////////////////////  
///           MAC nro                                                                      //
/// 
#define  mac_6    0x73     ///     Last mac number  MSB mac numbers at ethernet_mega.c    ///
                           //      Not relevat with Ethershield  
static uint8_t mymac[6] = { 0x44,0x76,0x58,0x10,0x00,mac_6 };
                                                                                          ///
/////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  Serial.println("Start 20.3.2023");
  fetch_IP();
}

void loop() {
  delay(1400);
  lcd.clear();
  lcd.print(Ethernet.localIP());
  }

void fetch_IP(void)
{
  byte rev=1;

  lcd.setCursor(0,1);
  //         01234567890123456789  
  lcd.print("     Waiting IP     ");

  rev=Ethernet.begin( mymac);                  // get IP number
     
  Serial.print( F("\nW5100 Revision ") );
    
  if ( rev == 0){
                   
                      Serial.println( F( "Failed to access Ethernet controller" ) );
                   
                                                // 0123456789012345689 
                    lcd.setCursor(0,0); lcd.print(" Ethernet failed   ");
                 }    
                 
              
     Serial.println( F( "Setting up DHCP" ));
     Serial.print("Connected with IP: "); 
     Serial.println(Ethernet.localIP()); 


  lcd.setCursor(0,1);
  //         012345678901234567890
  lcd.print("                     ");
  
  lcd.setCursor(0,1);
  lcd.print("myIP=");
  lcd.print(Ethernet.localIP());
  delay(1500);
}
