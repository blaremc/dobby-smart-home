
#include <SPI.h>
#include <Ethernet.h>

// подключаем библиотеку
#include <dht.h>

// Ethernet Configuration
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x16 };
IPAddress ip(192,168,1,16);
EthernetServer server(80);
#define REQUESTSIZE 30


#define PARAMSSIZE 8
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


#define LED2REDPIN 9
#define LED2GREENPIN 10
#define LED2BLUEPIN 11

#define TEMPPIN 0


#define LIGHTPIN 1 // сенсор света
#define MOTIONPIN 2 // сенсор движения
//#define IRTPIN 7 // ИК передатчик
#define IRRPIN 8 // ИК приемник
#define RELE1PIN 7 // Реле 1
#define RELE2PIN 4 // Реле 2


byte MOTIONVALUE = 0;
int MOTIONTIME = 0;
int MOTIONDELAY = 1000;

byte RELE1VALUE = 1;
byte RELE2VALUE = 1;

int TEMPVALUE = 0;
int HUMIDITYVALUE = 0;
// создаём объект-сенсор
DHT sensor = DHT();

// Storage for the recorded code
int codeType = -1; // The type of code
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state


EthernetClient client;

void setup(void) {
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.begin(9600); 
   pinMode(LED1REDPIN, OUTPUT);
  pinMode(LED1GREENPIN, OUTPUT);
  pinMode(LED1BLUEPIN, OUTPUT);
  pinMode(LED2REDPIN, OUTPUT);
  pinMode(LED2GREENPIN, OUTPUT);
  pinMode(LED2BLUEPIN, OUTPUT);
  sensor.attach(A0);
  pinMode(LIGHTPIN, OUTPUT);
  pinMode(MOTIONPIN, INPUT);
  pinMode(RELE1PIN, OUTPUT);
  pinMode(RELE2PIN, OUTPUT);
  digitalWrite(RELE1PIN, LOW); 
  digitalWrite(RELE2PIN, LOW); 
  
}

void act(){
  
  

  
  // METHOD - name of method
  // PARAMS - array of params
  
  if (strcmp(METHOD, "setRele") == 0){  
    if (atoi(PARAMS[0])==1){
      RELE1VALUE = atoi(PARAMS[1]);
      if (atoi(PARAMS[1])==0){       
        digitalWrite(RELE1PIN, HIGH); 
      } else {      
        digitalWrite(RELE1PIN, LOW); 
      }
    }
    if (atoi(PARAMS[0])==2){
      RELE2VALUE = atoi(PARAMS[1]);
      if (atoi(PARAMS[1])==0){
        digitalWrite(RELE2PIN, HIGH); 
      } else {
        digitalWrite(RELE2PIN, LOW); 
      }
    }
  }
  if (strcmp(METHOD, "getRele") == 0){  
    if (atoi(PARAMS[0])==1){
      client.println(RELE1VALUE);  
    }
    if (atoi(PARAMS[0])==2){
      client.println(RELE2VALUE);   
    }
  }
  if (strcmp(METHOD, "getTemperature") == 0){  
    
    getTemperature();
    client.print(TEMPVALUE);    
    client.print("C ");    
    client.print(HUMIDITYVALUE);    
    client.println("%");
  }  
  
  
  if (strcmp(METHOD, "getLight") == 0){  
    client.println(analogRead(LIGHTPIN));    
  }  
  if (strcmp(METHOD,  "getMotion")== 0){  
    client.println(MOTIONVALUE);    
  }
  
  if (strcmp(METHOD, "getLed") == 0){  
    if (atoi(PARAMS[0]) == 1){
      client.print(LED1REDTARGET);
      client.print(":");
      client.print(LED1GREENTARGET);
      client.print(":");
      client.println(LED1BLUETARGET);
    }
    if (atoi(PARAMS[0]) == 2){
      client.print(LED2REDTARGET);
      client.print(":");
      client.print(LED2GREENTARGET);
      client.print(":");
      client.println(LED2BLUETARGET);
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
     if (atoi(PARAMS[0]) == 2){
        LED2REDTARGET = atoi(PARAMS[1]);
        LED2GREENTARGET = atoi(PARAMS[2]);
        LED2BLUETARGET = atoi(PARAMS[3]);
      
        LED2SMOOTH = atoi(PARAMS[4]);
        if (LED2SMOOTH==0){
         LED2SMOOTH = 1; 
        } 
    }
    
  } 
  
}




void loop(void) {
   
  client = server.available();
  
  iteration++;
  if (iteration > 10000){
    iteration = 1;
  }
  
  // Датчик движения
  byte motion_value = digitalRead(MOTIONPIN);
  if (motion_value == 1){
    MOTIONTIME = MOTIONDELAY;
    MOTIONVALUE = 1;
 
  } else {
    if (MOTIONTIME>0){
      MOTIONTIME --;
      if (MOTIONTIME <=0){
        MOTIONVALUE = 0;
      }
    }
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
    
        
  if(client){
           
     char request[REQUESTSIZE];
     byte reqInd = 0;
     boolean currentLineIsBlank = true;
     int connectLoop = 0;

     while (client.connected() && connectLoop<10000) {

     if (client.available()) {
        connectLoop = 0;

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
      connectLoop++;

    }
    delay(1);
    client.stop();
  }
}

void getTemperature(){
  
   // метод update заставляет сенсор выдать текущие измерения
    sensor.update();
 
    switch (sensor.getLastError())
    {
        case DHT_ERROR_OK:
            TEMPVALUE = sensor.getTemperatureInt();
            HUMIDITYVALUE = sensor.getHumidityInt();       
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



