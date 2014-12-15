#include <SoftwareSerial.h>


int rele1 = 8;
int rele2 = 10; // R petal on RGB LED module connected to digital pin 11
byte com = 0; //reply from voice recognition

void setup() 
{ 
// Serial communication Speed 
Serial.begin(9600);

  
  pinMode(rele1, OUTPUT); // sets the ledPin to be an output
  pinMode(rele2, OUTPUT); // sets the redPin to be an output
  digitalWrite(rele1, HIGH);  
  digitalWrite(rele2, HIGH);  
  
  delay(2000);
  //Device Setup via AT commands 
  Serial.write(0xAA);
  Serial.write(0x37); // Switch module into compact mode   
  delay(1000);
  Serial.write(0xAA);
  Serial.write(0x21); // Import Group1 of 5 words 
  
  Serial.println(" Please say which environment you would like to control");
} 

void loop() // run over and over again
{ 

while(Serial.available())
{
com = Serial.read();
Serial.println(com,HEX);

switch(com)
{
case 0x11:
 digitalWrite(rele1, HIGH);
 digitalWrite(rele2, HIGH);
 Serial.println();
break;

case 0x12:
 digitalWrite(rele1, LOW);  
 digitalWrite(rele2, HIGH);
break;

case 0x13:
digitalWrite(rele1, HIGH);
digitalWrite(rele2, LOW);
break;

case 0x14:
digitalWrite(rele2, LOW);
digitalWrite(rele2, LOW);
break;

case 0x15:
 digitalWrite(rele1, LOW);  
 digitalWrite(rele2, HIGH);
 delay(500);
 digitalWrite(rele1, HIGH);
digitalWrite(rele2, LOW);
break;

}
}

} 


