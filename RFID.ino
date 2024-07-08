#include <SPI.h>
#include <MFRC522.h>
#include <WiFiClient.h>
constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
int A=0,B=0;
void verify_voter();

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  pinMode(2,INPUT);          
  // put your setup code here, to run once:
}

void loop() {
verify_voter();  
  // put your main code here, to run repeatedly:
}

void verify_voter()
{
   Serial.print("Please Plase your card: ");
   
   if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    //Serial.println(tag);
    if (tag == "32491147") {
      Serial.println(tag+ "  Access Granted!");
      vote();
    } else {
      Serial.println(tag+" , Access Denied!");
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    }
}


void  vote()
{
int flag=1;
while(flag)
{    
  int a=digitalRead(2);
  int b;
  Serial.println(a);

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

Serial.print("A: ");
Serial.print(A);
Serial.print("B: ");
Serial.println(B);
}//end loop

}//end reading votes