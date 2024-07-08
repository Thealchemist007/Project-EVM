

int A=0,B=0;
void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
pinMode(2,INPUT);
pinMode(3,INPUT);
}

void loop() {
//put your main code here, to run repeatedly:


}//end void loop

void  vote()
{
int a=digitalRead(2);
int b=digitalRead(3);
Serial.println(a);
delay(500);

if(a==1)
{
   while(a==1)
   {
     a=digitalRead(2);
   }
   A=A+1;
}

if(b==1)
{
  while(b==1)
  {
    b=digitalRead(3);
  }
  B=B+1;
}
Serial.print("A: ");
Serial.print(A);
Serial.print("B: ");
Serial.println(B);
}