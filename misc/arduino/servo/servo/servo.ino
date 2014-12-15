#include <Servo.h>


Servo servoMain; // Обьект Servo
 int pos = 0;
 
void setup()
{
   servoMain.attach(10); // Servo присоединен к 10 выводу
}
 
void loop()
{
   servoMain.writeMicroseconds(1300);
   delay(4000);          // Пауза 5 сек.
   servoMain.writeMicroseconds(1500);
   delay(10000);
   servoMain.writeMicroseconds(1700);
   delay(4000);          // Пауза 5 сек.
   servoMain.writeMicroseconds(1500);
   delay(10000);
}
