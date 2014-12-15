#include <dht.h>

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include "Ultrasonic.h"
#include <Servo.h>


// Ethernet Configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,3,2);
EthernetServer server(80);
Servo servoMain; // Обьект Servo

DHT sensor = DHT();

int RelayWaterColdOpen = 6;
int RelayWaterColdClose = 7;
int RelayWaterHotOpen = 8;
int RelayWaterHotClose = 9;
 
// sensor connected to:
// Trig - 2, Echo - 3
Ultrasonic ultrasonic(2, 3);



void setup(void) {
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.begin(9600);   
 
  sensor.attach(A4);
 
  pinMode(RelayWaterHotOpen, OUTPUT);     
  pinMode(RelayWaterHotClose, OUTPUT); 
  pinMode(RelayWaterColdOpen, OUTPUT);     
  pinMode(RelayWaterColdClose, OUTPUT);     
  digitalWrite(RelayWaterHotClose, HIGH);
  digitalWrite(RelayWaterHotOpen, HIGH);
  digitalWrite(RelayWaterColdOpen, HIGH);
  digitalWrite(RelayWaterColdClose, HIGH);  
  servoMain.attach(4); // Servo присоединен к 4 выводу
  servoMain.writeMicroseconds(1500);
}

void loop(void) {
 
  
  EthernetClient client = server.available();
  float value = ultrasonic.Ranging(CM); 
           
  if(client){
     
     Serial.println("new client");
     String request;
     
     boolean currentLineIsBlank = true;
     while (client.connected()) {
      
     if (client.available()) {
        char c = client.read();               
        
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
           request = getRequest(request);         
           sendHeaders(client);          
         Serial.print("request=");
                  Serial.println(request);
            
            
            
           if (request == "getTemperature"){
                 sensor.update();
                  if (sensor.getLastError() == DHT_ERROR_OK){
                     float value =  sensor.getTemperatureInt();
                     Serial.println(value);
                     client.print(value);
                  }else {
                    switch (sensor.getLastError())
                    {
                       
                        case DHT_ERROR_START_FAILED_1:
                            Serial.println("Error: start failed (stage 1)");
                            break;
                        case DHT_ERROR_START_FAILED_2:
                            Serial.println("Error: start failed (stage 2)");
                            break;
                        case DHT_ERROR_READ_TIMEOUT:
                            Serial.println("Error: read timeout");
                            break;
                        case DHT_ERROR_CHECKSUM_FAILURE:
                            Serial.println("Error: checksum error");
                            break;
                    }
                  }
           }
           
            if (request == "getDistance"){
                 float value = ultrasonic.Ranging(CM);
                 Serial.println(value);
                 client.print(value);
           }
           
           
           if (request.indexOf("setWaterhot")>=0){
                 String value = getValue(request);
                  switch(value.toInt()){
                  case 0:
                    digitalWrite(RelayWaterHotClose, HIGH);
                    digitalWrite(RelayWaterHotOpen, HIGH);
                    break;
                  case 1:
                    digitalWrite(RelayWaterHotOpen, LOW);   // реле включено
                    digitalWrite(RelayWaterHotClose, HIGH);
                    break;
                  case -1:
                    digitalWrite(RelayWaterHotClose, LOW);   // реле включено
                    digitalWrite(RelayWaterHotOpen, HIGH);
                    break;
                  }            

                 Serial.println("setWaterhot");
                 Serial.println(value);
                 client.print(value);
           }
           
           
           
           if (request.indexOf("setWatercold")>=0){
                 String value = getValue(request);
                  switch(value.toInt()){
                  case 0:
                    digitalWrite(RelayWaterColdClose, HIGH);
                    digitalWrite(RelayWaterColdOpen, HIGH);
                    break;
                  case 1:
                    digitalWrite(RelayWaterColdOpen, LOW);   // реле включено
                    digitalWrite(RelayWaterColdClose, HIGH);
                    break;
                  case -1:
                    digitalWrite(RelayWaterColdClose, LOW);   // реле включено
                    digitalWrite(RelayWaterColdOpen, HIGH);
                    break;
                  }            

                 Serial.println("setWatercold");
                 Serial.println(value);
                 client.print(value);
           }
           
           
           
           
           if (request.indexOf("setSink")>=0){
                 String value = getValue(request);
                  switch(value.toInt()){
                  case 0:
                    servoMain.writeMicroseconds(1500);
                    break;
                  case 1:
                    servoMain.writeMicroseconds(1400);
                    break;
                  case -1:
                    servoMain.writeMicroseconds(1600);
                    break;
                  }            

                 Serial.println("setSink");
                 Serial.println(value);
                 client.print(value);
           }
      
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          request += c;
          currentLineIsBlank = false;
        }
      }
    }
     
     
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}


float getDistance(){
  return ultrasonic.Ranging(CM); 
}



void sendHeaders(EthernetClient client){
     // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();
}

String getRequest(String request){
  request = request.substring( 
            request.indexOf(' ') + 2, 
            request.lastIndexOf(' '));
  request = request.substring( 0,
            request.indexOf(' '));  
  return request;
}
String getValue(String request){
  return request.substring(request.indexOf(':') + 1);
}
