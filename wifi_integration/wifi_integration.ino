#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
#include <WiFiClient.h>

/*-----------------ESP8266 Serial WiFi Module---------------*/
#define SSID "OPPOA9"     // "SSID-WiFiname" 
#define PASS "10270900"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip  192.168.101.108  184.106.153.149
String msg = "GET https://api.thingspeak.com/update?api_key=Z5T14WOYOOQDES05&field1=0"; //change it with your key...
/*-----------------------------------------------------------*/

constexpr uint8_t RST_PIN = 9;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;     // Configurable, see typical pin layout above

struct database{
    String uid;
    int record;
}d[5];


MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
int A=0,B=0,cap=5,total=0;
int error;
void verify_voter();
void print_details();
boolean connectWiFi();
void updateTemp();

void setup() {
  
  Serial.begin(115200);
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
  
  Serial.println("AT");//attention commands
  delay(5000);
  if(Serial.find("OK")){
    Serial.println("im in if loop after find");
    connectWiFi();
  }

}//end void setup


void loop() {
 start: //label 
  error=0;
  
  total=A+B;
  if(total==cap)
  {
    while(1)
    {
      Serial.println("Voting limit reached");
    }
  }

  verify_voter();  
  
  updateTemp();
  //Resend if transmission is not completed 
  if (error==1){
    goto start; //go to label "start"
  }

}//end void loop

void updateTemp(){

 int a=A;
 int b=B;

  String cmd = "AT+CIPSTART=\"TCP\",\"";//CIPSTART-Chip start
  cmd += IP;//String concatenation with previous String statement
  cmd += "\",80";//80 is the port number
  Serial.println(cmd);//String is finally printed on serial monitor
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field1=";    //field 1 for temperature,&field1 is variable in thingspeak cloud
  cmd += (String)a;
  cmd += "&field2=";  //field 2 for humidity
  cmd += (String)b;//convert humidity value to string and append to cmd
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");//uploading through CIPSEND
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }
  else{
    Serial.println("AT+CIPCLOSE");//CIPCLOSE-after sending data,instruction is used to close transmission
    //Resend...
    error=1;
  }
}

boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");//1 indicates that it is used as slave
  delay(2000);
  String cmd="AT+CWJAP=\"";//Check whether transmitted string length is same as received string length
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}


void verify_voter()
{
  int i,flag=0;

  lcd.clear(); 
  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("Enter your card");

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
      flag=1;
     }
   }

   if(flag==0)
   {
     Serial.print("Voter not found");
     
     lcd.clear(); 
     lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
     lcd.print("Voter not found");

      tag = "";
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      return;
   }


    for(i=0;i<5;i++)
    {
       if(tag==d[i].uid)
       {
         
         if(d[i].record==0)
         {
           
           lcd.clear(); 
           lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
           lcd.print("Allowed to vote");

          Serial.println("Allowed to vote! ");
          d[i].record=1;
          delay(1000);
          vote();
          break;
         }
         else
         {
           lcd.clear(); 
           lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
           lcd.print("Already voted");

          Serial.println("Valid but already voted");
          delay(1000);
         }

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
 
 lcd.clear(); 
 lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
 lcd.print("Vote casted");
 lcd.setCursor(0,1);   //Set cursor to character 2 on line 0
 lcd.print("Thank you");
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