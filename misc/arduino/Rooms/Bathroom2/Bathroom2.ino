
#include <IRremote.h>
#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>
#include "Ultrasonic.h"

// Ethernet Configuration
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x14};
IPAddress ip(192,168,1,14);
EthernetServer server(80);
#define REQUESTSIZE 30


#define PARAMSSIZE 8
char PARAMS[PARAMSSIZE][10];  
char METHOD[15];
int iteration;




byte LED1REDTARGET = 0;
byte LED1GREENTARGET = 0;
byte LED1BLUETARGET = 0;

byte LED1REDVALUE = 0;
byte LED1GREENVALUE = 0;
byte LED1BLUEVALUE = 0;
byte LED1SMOOTH = 0;

// Цифровые

#define DISTANCETRIG 2
#define DISTANCEECHO 3

#define RELE2PIN 4 // Реле 2
#define RELE1PIN 7 // Реле 1

#define LED1REDPIN 0
#define LED1GREENPIN 6
#define LED1BLUEPIN 9

#define MOTIONPIN 5 // сенсор движения


#define IRRPIN 0 // ИК приемник

//Аналоговые

#define LIGHTPIN 1 // сенсор света аналог
#define TEMPPIN A0 // сенсор температуры

IRrecv irrecv(IRRPIN);
decode_results results;
long IRCOMMANDS[10];

int RelayWaterColdOpen = 8;
int RelayWaterColdClose =10;
int RelayWaterHotOpen = 11;
int RelayWaterHotClose = 12;



// sensor connected to:
// Trig - 2, Echo - 3
Ultrasonic ultrasonic(DISTANCETRIG, DISTANCEECHO);
float DISTANCEVALUE = 0;

byte MOTIONVALUE = 0;
int MOTIONTIME = 0;
int MOTIONDELAY = 1000;

int LIGHTVALUE = -1;
int LIGHTTIME = 0;
int LIGHTDELAY = 300;

int TEMPERATUREVALUE = 0;
int HUMIDITYVALUE = 0;
byte RELE1VALUE = 1;
byte RELE2VALUE = 1;

// Storage for the recorded code
int codeType = -1; // The type of code
unsigned long codeValue; // The code value if not raw
unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state
DHT sensor = DHT();


EthernetClient client;

void setup(void) {
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.begin(9600); 
  //irrecv.enableIRIn();
  sensor.attach(TEMPPIN);
  pinMode(LED1REDPIN, OUTPUT);
  pinMode(LED1GREENPIN, OUTPUT);
  pinMode(LED1BLUEPIN, OUTPUT);
  
  pinMode(LIGHTPIN, INPUT);
  pinMode(MOTIONPIN, INPUT);
  pinMode(RELE1PIN, OUTPUT);
  pinMode(RELE2PIN, OUTPUT);
  digitalWrite(RELE1PIN, LOW); 
  digitalWrite(RELE2PIN, LOW); 
   
  
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
    return;
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
    return;
  } 

  
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
    return;
  }
  
  if (strcmp(METHOD, "getDistance") == 0){  
    DISTANCEVALUE = ultrasonic.Ranging(CM);
    client.print(DISTANCEVALUE);
    return;    
  } 
  

  if (strcmp(METHOD, "getTemperature") == 0){  
    getTemperature();
    client.print(TEMPERATUREVALUE);    
    client.print("C ");    
    client.print(HUMIDITYVALUE);    
    client.print("%");   
    return; 
  } 
  if (strcmp(METHOD, "getLight") == 0){     
    client.println(LIGHTVALUE);   
    return;
  }  
  if (strcmp(METHOD,  "getMotion")== 0){  
    client.println(MOTIONVALUE);    
    return;
  }
  
  if (strcmp(METHOD,  "getIR")== 0){  
    
    client.println(codeType);
    client.println(codeValue);
 /*
    for (int i =0; i<10; i++){
      client.print(IRCOMMANDS[i]);
      client.print(" ");
    } */    
     return;
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
     return;
  } 
  
  client.println("Bathroom Arduino");    
  client.println("Version 1.0"); 
  
}




void loop(void) {
   
  client = server.available();
  
  iteration++;
  if (iteration > 10000){
    iteration = 1;
  }
  
  // Датчик движения
  byte motion_value = digitalRead(MOTIONPIN);
  if (motion_value == HIGH){
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
  
  LIGHTTIME++;
   
  if (LIGHTTIME >= LIGHTDELAY) {
   LIGHTTIME = 0;
   LIGHTVALUE = analogRead(LIGHTPIN);
  }
  

    // ИК приемник
  if (irrecv.decode(&results)) // Если данные пришли 
  {
    Serial.println(results.value, HEX); // Отправляем полученную данную в консоль
    for (int i=0; i<9; i++){
      IRCOMMANDS[i] = IRCOMMANDS[i+1];
    }    
    IRCOMMANDS[9] = results.value;
    storeCode(&results);
    irrecv.resume(); // Принимаем следующую команду
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
}


void sendHeaders(EthernetClient client){
     // send a standard http response header
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();
}

char *getRequest(char *request) {

  request = strstr(request,"/");    
  request = strtok (request," ");  
  request = request + 1;
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
            TEMPERATUREVALUE = sensor.getTemperatureInt();
            HUMIDITYVALUE = sensor.getHumidityInt();
    
            break;
        case DHT_ERROR_START_FAILED_1:
            client.println("Error: start failed (stage 1)");
            break;
        case DHT_ERROR_START_FAILED_2:
            client.println("Error: start failed (stage 2)");
            break;
        case DHT_ERROR_READ_TIMEOUT:
            client.println("Error: read timeout");
            break;
        case DHT_ERROR_CHECKSUM_FAILURE:
            client.println("Error: checksum error");
            break;
    }
 
}

void storeCode(decode_results *results) {
  codeType = results->decode_type;
  int count = results->rawlen;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, saving as raw");
    codeLen = results->rawlen - 1;
    // To store raw codes:
    // Drop first value (gap)
    // Convert from ticks to microseconds
    // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" m");
      } 
      else {
        // Space
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(rawCodes[i - 1], DEC);
    }
    Serial.println("");
  }
  else {
    if (codeType == NEC) {
      Serial.print("Received NEC: ");
      if (results->value == REPEAT) {
        // Don't record a NEC repeat value as that's useless.
        Serial.println("repeat; ignoring.");
        return;
      }
    } 
    else if (codeType == SONY) {
      Serial.print("Received SONY: ");
    } 
    else if (codeType == RC5) {
      Serial.print("Received RC5: ");
    } 
    else if (codeType == RC6) {
      Serial.print("Received RC6: ");
    } 
    else {
      Serial.print("Unexpected codeType ");
      Serial.print(codeType, DEC);
      Serial.println("");
    }
    Serial.println(results->value, HEX);
    codeValue = results->value;
    codeLen = results->bits;
  }
}



