#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
#include <WiFiClient.h>
constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

struct database{
    String uid;
    int record;
}d[5];


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
  
  pinMode(4,OUTPUT);    
 
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  digitalWrite(4,HIGH);
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
    
  // Print a message on both lines of the LCD.
  // put your setup code here, to run once:
  /*initialize all recodto 0*/
  int i;
  for(i=0;i<5;i++)
  {
    d[i].record=0;
  }
  d[0].uid="99104814";
  d[1].uid="32491147";
  d[2].uid="211137253165";
  d[3].uid="161372689";
  d[4].uid="123885153";
}//void setup

void loop() {
//print_details();
verify_voter();  
  // put your main code here, to run repeatedly:
}

void verify_voter()
{
  int i;

   Serial.println("Please Plase your card: ");
   Serial.print(digitalRead(2));
   Serial.print(" ");
   Serial.print(digitalRead(3));
   Serial.print(" ");
   Serial.print(digitalRead(5));
   Serial.print(" ");
   Serial.print(digitalRead(6));
   
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
  
    Serial.print("UID: ");
    Serial.println(tag);
    delay(1000); 
    for(i=0;i<5;i++)
    {
       if(tag==d[i].uid)
       {
         if(d[i].record==0)
         {
          Serial.println("Allowed to vote! ");
          d[i].record=1;
          delay(1000);
          vote();
         }
         else
         {
          Serial.println("Valid but already voted");
          delay(1000);
         }

       }
       else
       {
         Serial.println("Voter not found");
       }

    } 
   
   
   
  /*  if (tag == "32491147") {
      Serial.println(tag+ "  Access Granted!");
      
      delay(10000);
    } else {
      Serial.println(tag+" , Access Denied!");
      delay(10000);
    } */
   
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
    int a=digitalRead(5);
    int b=digitalRead(6);
    Serial.print(a);
    Serial.print(" ");
    Serial.println(b);

  if(a==1)
  {
   while(a==1)
   {
     a=digitalRead(2);
   }
   A=A+1;
   break;
  }

  if(b==1)
  {
    while(b==1)
    {
     b=digitalRead(3);
    }
   B=B+1;
   break;
  }

 }//end loop

Serial.print("A: \t");
Serial.print(A);
Serial.print("\tB: \t");
Serial.println(B);
delay(3000);
}


void print_details()
{
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("  Welcome !     ");
  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print("Enter your vote "); 
  //vote();
  lcd.setCursor(0,0);
  lcd.print("  your vote is");
  lcd.setCursor(0,1);
  lcd.print(" appreciated !   ");
  delay(3000);
  //loop();
}//end reading votes