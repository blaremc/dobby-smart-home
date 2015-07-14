
#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>
#include "Ultrasonic.h"

// Ethernet Configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,10);
EthernetServer server(80);
#define REQUESTSIZE 30
DHT sensor = DHT();
// sensor connected to:
// Trig - 2, Echo - 3
Ultrasonic ultrasonic(2, 3);




#define PARAMSSIZE 10
char PARAMS[PARAMSSIZE][10];  
char METHOD[15];
int iteration;



#define LED1REDPIN 3
#define LED1GREENPIN 5
#define LED1BLUEPIN 6

byte LED1REDTARGET = 0;
byte LED1GREENTARGET = 0;
byte LED1BLUETARGET = 0;

byte LED1REDVALUE = 0;
byte LED1GREENVALUE = 0;
byte LED1BLUEVALUE = 0;
byte LED1SMOOTH = 0;

byte LED2REDTARGET = 0;
byte LED2GREENTARGET = 0;
byte LED2BLUETARGET = 0;

byte LED2REDVALUE = 0;
byte LED2GREENVALUE = 0;
byte LED2BLUEVALUE = 0;
byte LED2SMOOTH = 0;

byte LED3TARGET = 0;
byte LED3VALUE = 0;
byte LED3SMOOTH = 0;

byte LED4TARGET = 0;
byte LED4VALUE = 0;
byte LED4SMOOTH = 0;

byte LED5TARGET = 0;
byte LED5VALUE = 0;
byte LED5SMOOTH = 0;

byte LED6TARGET = 0;
byte LED6VALUE = 0;
byte LED6SMOOTH = 0;

byte LED7TARGET = 0;
byte LED7VALUE = 0;
byte LED7SMOOTH = 0;

byte LED8TARGET = 0;
byte LED8VALUE = 0;
byte LED8SMOOTH = 0;

#define LED2REDPIN 9
#define LED2GREENPIN 10
#define LED2BLUEPIN 11

#define LED3PIN 3
#define LED4PIN 5
#define LED5PIN 6
#define LED6PIN 9
#define LED7PIN 10
#define LED8PIN 11

int TEMPERATUREVALUE = 0;
int HUMIDITYVALUE = 0;

int RelayWaterColdOpen = 7;
int RelayWaterColdClose =8;
int RelayWaterHotOpen = 9;
int RelayWaterHotClose = 10;
 EthernetClient client;

void setup(void) {
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.begin(9600); 
 
  pinMode(LED1REDPIN, OUTPUT);
  pinMode(LED1GREENPIN, OUTPUT);
  pinMode(LED1BLUEPIN, OUTPUT);
  
  sensor.attach(A4);
  pinMode(RelayWaterHotOpen, OUTPUT);     
  pinMode(RelayWaterHotClose, OUTPUT); 
  pinMode(RelayWaterColdOpen, OUTPUT);     
  pinMode(RelayWaterColdClose, OUTPUT);     
  digitalWrite(RelayWaterHotClose, HIGH);
  digitalWrite(RelayWaterHotOpen, HIGH);
  digitalWrite(RelayWaterColdOpen, HIGH);
  digitalWrite(RelayWaterColdClose, HIGH);  
}

void act(){
  
  // METHOD - name of method
  // PARAMS - array of params
  
  if (strcmp(METHOD, "getTemperature") == 0){  
    getTemperature();
    client.print(TEMPERATUREVALUE);    
    client.print("C ");    
    client.print(HUMIDITYVALUE);    
    client.print("%");    
  } 
  
  if (strcmp(METHOD, "setWaterhot") == 0){  
       switch(atoi(PARAMS[0])){
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
  } 
  
  
    if (strcmp(METHOD, "setWatercold") == 0){  
       switch(atoi(PARAMS[0])){
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
  } 
  
  
if (strcmp(METHOD, "setLed")== 0){
  
    if (atoi(PARAMS[0]) == 1){
      
      LED1REDTARGET = atoi(PARAMS[1]);
      LED1GREENTARGET = atoi(PARAMS[2]);
      LED1BLUETARGET = atoi(PARAMS[3]);

      LED1SMOOTH = atoi(PARAMS[4]);
      if (LED1SMOOTH==0){
       LED1SMOOTH = 1; 
      }
    
    }
       
  } 
  
}




void loop(void) {
   
  EthernetClient client = server.available();
  
  iteration++;
  if (iteration > 10000){
    iteration = 1;
  }
  
  
  if (LED1REDTARGET != LED1REDVALUE || LED1GREENTARGET != LED1GREENVALUE || LED1BLUETARGET != LED1BLUEVALUE){
    
    if (iteration % LED1SMOOTH == 0){
      if (LED1REDTARGET != LED1REDVALUE){   
         LED1REDTARGET > LED1REDVALUE? LED1REDVALUE++ : LED1REDVALUE--;
         analogWrite(LED1REDPIN, LED1REDVALUE);
      }    
      if (LED1GREENTARGET != LED1GREENVALUE){   
         LED1GREENTARGET > LED1GREENVALUE? LED1GREENVALUE++ : LED1GREENVALUE--;
         analogWrite(LED1GREENPIN, LED1GREENVALUE);
      }
      if (LED1BLUETARGET != LED1BLUEVALUE){   
         LED1BLUETARGET > LED1BLUEVALUE? LED1BLUEVALUE++ : LED1BLUEVALUE--;
         analogWrite(LED1BLUEPIN, LED1BLUEVALUE);       
      }
    }
  }
  
    if (LED2REDTARGET != LED2REDVALUE || LED2GREENTARGET != LED2GREENVALUE || LED2BLUETARGET != LED2BLUEVALUE){
    
    if (iteration % LED2SMOOTH == 0){
      if (LED2REDTARGET != LED2REDVALUE){   
         LED2REDTARGET > LED2REDVALUE? LED2REDVALUE++ : LED2REDVALUE--;
         analogWrite(LED2REDPIN, LED2REDVALUE);
      }    
      if (LED2GREENTARGET != LED2GREENVALUE){   
         LED2GREENTARGET > LED2GREENVALUE? LED2GREENVALUE++ : LED2GREENVALUE--;
         analogWrite(LED2GREENPIN, LED2GREENVALUE);
      }
      if (LED2BLUETARGET != LED2BLUEVALUE){   
         LED2BLUETARGET > LED2BLUEVALUE? LED2BLUEVALUE++ : LED2BLUEVALUE--;
         analogWrite(LED2BLUEPIN, LED2BLUEVALUE);       
      }
    }
  }
  
   if (LED3TARGET != LED3VALUE){
     if (iteration % LED3SMOOTH == 0){
        LED3TARGET > LED3VALUE? LED3VALUE++ : LED3VALUE--;
         analogWrite(LED3PIN, LED3VALUE);
     }     
   }
   if (LED4TARGET != LED4VALUE){
     if (iteration % LED4SMOOTH == 0){
        LED4TARGET > LED4VALUE? LED4VALUE++ : LED4VALUE--;
         analogWrite(LED4PIN, LED4VALUE);
     }     
   }
   if (LED5TARGET != LED5VALUE){
     if (iteration % LED5SMOOTH == 0){
        LED5TARGET > LED5VALUE? LED5VALUE++ : LED5VALUE--;
         analogWrite(LED5PIN, LED5VALUE);
     }     
   }
   if (LED6TARGET != LED6VALUE){
     if (iteration % LED6SMOOTH == 0){
        LED6TARGET > LED6VALUE? LED6VALUE++ : LED6VALUE--;
         analogWrite(LED6PIN, LED6VALUE);
     }     
   }
   if (LED7TARGET != LED7VALUE){
     if (iteration % LED7SMOOTH == 0){
        LED7TARGET > LED7VALUE? LED7VALUE++ : LED7VALUE--;
         analogWrite(LED7PIN, LED7VALUE);
     }     
   }
   if (LED8TARGET != LED8VALUE){
     if (iteration % LED8SMOOTH == 0){
        LED8TARGET > LED8VALUE? LED8VALUE++ : LED8VALUE--;
         analogWrite(LED8PIN, LED8VALUE);
     }     
   }
  
        
   if(client){
           
     char request[REQUESTSIZE];
     byte reqInd = 0;
     boolean currentLineIsBlank = true;
     while (client.connected()) {

     if (client.available()) {
        char c = client.read();        
        if (c == '\n' && currentLineIsBlank) {
          Serial.println(request);
           char* buf =getRequest(request);
           strcpy(request,buf);
           sendHeaders(client);          
           parseRequest(request);          
           act();       
                      
          break;
        }
        if (c == '\n') {
        
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {          
          if (reqInd>REQUESTSIZE){
            continue;
          }
          request[reqInd] = c;         
          reqInd++;
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
    getTemperature();
    delay(1);
    client.stop();
  }
}

void sendHeaders(EthernetClient client){
     // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();
}

char *getRequest(char *request) {
  Serial.print("request1=");
  Serial.println(request);

  request = strstr(request,"/");    
  request = strtok (request," ");  
  request = request + 1;
  Serial.println(request); 
  return request;
}


void parseRequest(char *request){
   Serial.print("request2=");
    Serial.println(request);
    strcpy(request , request);
  char *pos= strtok(request,":");
  Serial.println(request);
  Serial.println(pos);
   if (!pos){
     strcpy(METHOD , request);
   }else {
     strcpy(METHOD , pos);
   }
   
 
   for (int i=0; i<PARAMSSIZE; i++){
     memset(PARAMS[i], 0, sizeof PARAMS[i]);

   }
   
   Serial.print("METHOD='");
   Serial.print(METHOD);
   Serial.println("'"); 
  
  
   Serial.println(request); 
   
   int ind = 0;
  
    pos = strtok(NULL,":");
  
   if (pos!=NULL){
     while (pos!=NULL){
       strcpy(PARAMS[ind] , pos);       
       pos = strtok(NULL,":");
          
       Serial.print("PARAMS[");
       Serial.print(ind);
       Serial.print("]=");
       Serial.println(PARAMS[ind]);
       ind++;
     }
   }
}
void getTemperature(){
  
  // метод update заставляет сенсор выдать текущие измерения
    sensor.update();
 
    switch (sensor.getLastError())
    {
        case DHT_ERROR_OK:
    
             char msg[128];
            // данные последнего измерения можно считать соответствующими
            // методами
            sprintf(msg, "Temperature = %dC, Humidity = %d%%", 
                    sensor.getTemperatureInt(), sensor.getHumidityInt());
            Serial.println(msg);
            break;
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

