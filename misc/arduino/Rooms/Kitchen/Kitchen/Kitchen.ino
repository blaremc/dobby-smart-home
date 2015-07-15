#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>


#include <IRremote.h>
#include <SPI.h>
#include <Ethernet.h>

// Ethernet Configuration
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x16 };
IPAddress ip(192,168,1,16);
EthernetServer server(80);
EthernetClient client;

byte SERVER[] = { 192, 168, 1, 4 };
EthernetClient client_get;

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

char buf[80];

#define LED2REDPIN 9
#define LED2GREENPIN 10
#define LED2BLUEPIN 11




#define LIGHTPIN 1 // сенсор света
#define MOTIONPIN 2 // сенсор движения
//#define IRTPIN 7 // ИК передатчик
#define IRRPIN 8 // ИК приемник
#define RELE1PIN 7 // Реле 1
#define RELE2PIN 4 // Реле 2

IRrecv irrecv(IRRPIN);
decode_results results;
long IRCOMMANDS[10];

IRsend irsend;


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
char ipbuff[16];




void setup(void) {
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600); 
  irrecv.enableIRIn();
  pinMode(LED1REDPIN, OUTPUT);
  pinMode(LED1GREENPIN, OUTPUT);
  pinMode(LED1BLUEPIN, OUTPUT);
  pinMode(LED2REDPIN, OUTPUT);
  pinMode(LED2GREENPIN, OUTPUT);
  pinMode(LED2BLUEPIN, OUTPUT);
  
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
    return;
  }
  
  if (strcmp(METHOD, "getRele") == 0){  
    if (atoi(PARAMS[0])==1){
      client.println(RELE1VALUE);  
    }
    if (atoi(PARAMS[0])==2){
      client.println(RELE2VALUE);   
    }
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
    return;
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
     if (atoi(PARAMS[0]) == 2){
        LED2REDTARGET = atoi(PARAMS[1]);
        LED2GREENTARGET = atoi(PARAMS[2]);
        LED2BLUETARGET = atoi(PARAMS[3]);
      
        LED2SMOOTH = atoi(PARAMS[4]);
        if (LED2SMOOTH==0){
         LED2SMOOTH = 1; 
        } 
    }
    return;
  } 
  
  client.println("Kitchen Arduino");
  client.println("Version 1.2");
  client.print("MOTION = ");
  client.println(MOTIONVALUE);
  client.print("LIGHT = ");
  client.println(LIGHTVALUE);
  client.print("IR = ");
  client.print(codeType);
  client.print("_");
  client.println(codeValue);
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
    if (MOTIONVALUE == 0){
      sprintf(buf, "GET /ajax/events/?device=%s&value=%i", "motion", 1);   
      sentValueToServer();
    }
    MOTIONVALUE = 1; 
       
    
  } else {
    if (MOTIONTIME>0){
      MOTIONTIME --;
      if (MOTIONTIME <=0){
         if (MOTIONVALUE == 1){
          sprintf(buf, "GET /ajax/events/?device=%s&value=%i", "motion", 0);   
          sentValueToServer();
        }
        MOTIONVALUE = 0;
      }
    }
  }
  
  LIGHTTIME++;
   
  if (LIGHTTIME >= LIGHTDELAY) {
     LIGHTTIME = 0;
     
     int TEMPVALUE = analogRead(LIGHTPIN);
     if (abs(TEMPVALUE - LIGHTVALUE)>30) {      
        sprintf(buf, "GET /ajax/events/?device=%s&value=%i", "light",(int)LIGHTVALUE);   
        sentValueToServer();
        LIGHTVALUE = TEMPVALUE;
     }
   }
  
  
  // ИК приемник
  if (irrecv.decode(&results)) // Если данные пришли 
  {
    
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
     while (client.connected()) {

     if (client.available()) {
        char c = client.read();        
        if (c == '\n' && currentLineIsBlank) {
    
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

void sentValueToServer(){

    client_get.stop();    
    Serial.println("connecting...");
    int ret = client_get.connect(SERVER, 80);    
  if (ret) {
    Serial.println("connected");
    // Make a HTTP request:
   client_get.print(buf);
   client_get.println(" HTTP/1.0");
   client_get.println("Host: 192.168.1.4");
   client_get.println(ipbuff); // ip адрес нашего контроллера в текстовом виде
   client_get.print("Content-Type: text/html\n");
   client_get.println("Connection: close\n");
   delay(500);
   client_get.stop();
  }
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
    Serial.print("Error: ");
    Serial.println(ret);
    Serial.println(client_get.status());
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
  strcpy(request , request);
  char *pos= strtok(request,":");
   if (!pos){
     strcpy(METHOD , request);
   }else {
     strcpy(METHOD , pos);
   }
   
   for (int i=0; i<PARAMSSIZE; i++){
     memset(PARAMS[i], 0, sizeof PARAMS[i]);

   }   
   int ind = 0;
  
    pos = strtok(NULL,":");
  
   if (pos!=NULL){
     while (pos!=NULL){
       strcpy(PARAMS[ind] , pos);       
       pos = strtok(NULL,":");          
       ind++;
     }
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
    // Send data to server
    sprintf(buf, "GET /ajax/events/?device=%s&value=%i_%lu", "ir",codeType,(int)codeValue);   
    sentValueToServer();    
  }
}

