#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
#include <WiFiClient.h>
constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
int A=0,B=0;
void verify_voter();
void print_details();
void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(2,INPUT);        
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  // Print a message on both lines of the LCD.
  // put your setup code here, to run once:
}

void loop() {
print_details();
delay(2000);
vote();  
  // put your main code here, to run repeatedly:
}

// void verify_voter()
// {
//    Serial.print("Please Plase your card: ");
   
//    if ( ! rfid.PICC_IsNewCardPresent())
//     return;
//   if (rfid.PICC_ReadCardSerial()) {
//     for (byte i = 0; i < 4; i++) {
//       tag += rfid.uid.uidByte[i];
//     }
//     //Serial.println(tag);
//     if (tag == "32491147") {
//       Serial.println(tag+ "  Access Granted!");
//       vote();
//     } else {
//       Serial.println(tag+" , Access Denied!");
//     }
//     tag = "";
//     rfid.PICC_HaltA();
//     rfid.PCD_StopCrypto1();
//     }
// }


void  vote()
{
int flag=1;
while(flag)
{    
  int a=digitalRead(2);
  int b;
 // Serial.println(a);

if(a==1)
{
   while(a==1)
   {
     a=digitalRead(2);
   }
   A=A+1;
   flag=0;
}

if( b==1)
{
  while(b==1)
  {
    b=digitalRead(3);
  }
  flag=0;
  B=B+1;
}
Serial.print("A: \t");
Serial.print(A);
Serial.print("\tB: \t");
Serial.println(B);
delay(1000);
}//end loop

}
void print_details()
{
    lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("  Welcome !     ");
   
  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print("Enter your vote "); 
  vote();
  lcd.setCursor(0,0);
  lcd.print("  your vote is");
  lcd.setCursor(0,1);
  lcd.print(" appreciated !   ");
  delay(3000);
  loop();
}//end reading votes