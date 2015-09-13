#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

#include <SPI.h>
#include <Ethernet.h>

// Ethernet Configuration
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xA6 };
IPAddress ip(192,168,1,116);
EthernetServer server(80);
EthernetClient client;

byte SERVER[] = { 192, 168, 1, 4 };
EthernetClient client_get;

#define REQUESTSIZE 200

#define IRCOMMANDSSIZE 20
String IRCOMMANDS[IRCOMMANDSSIZE]; 

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




#define LIGHTPIN 1 // сенсор света
#define MOTIONPIN 2 // сенсор движения
//#define IRTPIN 7 // ИК передатчик
#define IRRPIN 8 // ИК приемник
#define RELE1PIN 7 // Реле 1
#define RELE2PIN 4 // Реле 2


byte MOTIONVALUE = 0;
int MOTIONTIME = 0;
int MOTIONDELAY = 1000;
long SENDTIME = 0;
long SENDDELAY = 5000;
long MINSENDDELAY = 5000;
long MAXSENDDELAY = 200000;

long IRGETDELAY = 200000;
long IRSENDTIME = 0;

int LIGHTVALUE = -1;
int LIGHTTIME = 0;
int LIGHTDELAY = 1000;

int TEMPERATUREVALUE = 0;
int HUMIDITYVALUE = 0;
byte RELE1VALUE = 1;
byte RELE2VALUE = 1;

// Storage for the recorded code
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state
char ipbuff[16];

char buf[80];
byte isclearbuf = 1;


void setup(void) {
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600); 
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

  int res = client_get.connect(SERVER, 80);
  if (res){
    Serial.println("Success connection");
  } else {
    Serial.println("Fail connection");  
  }
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
  client.println("Version 2.0");
  client.print("MOTION = ");
  client.println(MOTIONVALUE);
  client.print("LIGHT = ");
  client.println(LIGHTVALUE);
  client.print("String = ");
  client.println(buf);
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
      sprintf(buf, "%sdevice[]=%s&value[]=%i&", buf, "motion",1);
      isclearbuf = 0;      
    }
    MOTIONVALUE = 1;
  
  } else {
    if (MOTIONTIME>0){
      MOTIONTIME --;
      if (MOTIONTIME <=0){
         if (MOTIONVALUE == 1){
          sprintf(buf, "%sdevice[]=%s&value[]=%i&", buf, "motion",0);
                 isclearbuf = 0;
          
        }
       MOTIONVALUE = 0;
      }
    }
  }
  
  LIGHTTIME++;
   
  if (LIGHTTIME >= LIGHTDELAY) {
     LIGHTTIME = 0;
     
     int TEMPVALUE = analogRead(LIGHTPIN);
     if (( TEMPVALUE<100 && abs(TEMPVALUE - LIGHTVALUE)>30) || ( TEMPVALUE>=100 && abs(TEMPVALUE - LIGHTVALUE)>50)) {  
        sprintf(buf, "%sdevice[]=%s&value[]=%i&", buf, "light",(int)TEMPVALUE); 
           isclearbuf = 0;   
        
        
        LIGHTVALUE = TEMPVALUE;
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

      if (LED1REDVALUE ==0 && LED1GREENVALUE ==0 && LED1BLUEVALUE==0){
        digitalWrite(RELE2PIN, HIGH);          
      } else {      
        digitalWrite(RELE2PIN, LOW);     
      }
       if (LED1REDVALUE ==255 || LED1GREENVALUE ==255 || LED1BLUEVALUE==255){
        digitalWrite(RELE1PIN, LOW);     
       }else {
        digitalWrite(RELE1PIN, HIGH);             
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

     getServerResponse(request, client);
     
     sendHeaders(client);       
       bool again = false;
       bool res = true;
       while (res){   
         res = parseRequest(request, again);
         Serial.print("res ");
         Serial.println(res);
         again = true;
         act();       
       }

    
    delay(1);
    client.stop();
  }
  LIGHTTIME++;
  SENDTIME++;
  IRSENDTIME++;
  if (SENDTIME >= SENDDELAY) {
    SENDTIME = 0;
    sendToServer();  
  }


if (IRSENDTIME >= IRGETDELAY) {
    IRSENDTIME = 0;
    getIRCommand();  
  }

}


void getServerResponse(char* request , EthernetClient client){

    
     byte reqInd = 0;
     boolean currentLineIsBlank = true;

 while (client.connected()) {

     if (client.available()) {
        char c = client.read();        
        if (c == '\n' && currentLineIsBlank) {
    
           char* cbuf =getRequest(request);
           strcpy(request,cbuf);
           sendHeaders(client);       
      
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
}



void getIRCommand() {

    int res = 1;
    client_get.stop();        
    res = client_get.connect(SERVER, 80);
    Serial.println("getIRCommand");
    if (res){
      Serial.println("Success connection");
    } else {
      Serial.println("Fail connection");  
    }
          
    if (res){
     client_get.print("GET /ajax/ircommand");    
     client_get.println(" HTTP/1.1");
     client_get.println("Host: 192.168.1.2");
     client_get.println("Connection: keep-alive");
     client_get.println("Keep-Alive: timeout=30, max=100");
     client_get.println("User-Agent: arduino-ethernet");
     client_get.println("Content-Type: text/html\n");
     delay(500);
     char request[REQUESTSIZE];
     getServerResponse(request, client);
     Serial.println(request);
    }  
}

void sendToServer() {
  
  if (!isclearbuf){    
      int res = 1;
         client_get.stop();
        
     //   Serial.println("conecting...");
        res = client_get.connect(SERVER, 80);
        if (res){
          Serial.println("Success connection");
        } else {
          Serial.print("Fail connection ");  
          Serial.println(SENDDELAY);  
          Serial.println(buf);  
        }
        
      
    if (res){
     Serial.print("GET /ajax/events/?");
     Serial.println(buf);
      // Make a HTTP request:
     client_get.print("GET /ajax/events/?");    
     client_get.print(buf);
     client_get.println(" HTTP/1.1");
     client_get.println("Host: 192.168.1.4");
     client_get.println("Connection: keep-alive");
     client_get.println("Keep-Alive: timeout=30, max=100");
     client_get.println("User-Agent: arduino-ethernet");
     client_get.println("Content-Type: text/html\n");
     delay(500);
     //client_get.stop();
    
     isclearbuf = 1;
     sprintf(buf, "");
     Serial.println("sended");
     SENDDELAY = MINSENDDELAY;
    }else {
     if (sizeof(buf)> 100){
        sprintf(buf, "");
     }
     if (SENDDELAY < MAXSENDDELAY){
      SENDDELAY += 2000;
     }
    }
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


bool parseRequest(char *request, bool again){
  strcpy(request , request);
   char *pos;
  if (again) {
   pos= strtok(NULL, ":");
  }else {
   pos= strtok(request, ":");
  }
   if (!pos){
     strcpy(METHOD , request);
   }else {
     strcpy(METHOD , pos);
   }
     Serial.print("METHOD ");
       Serial.println(METHOD);
   
   for (int i=0; i<PARAMSSIZE; i++){
     memset(PARAMS[i], 0, sizeof PARAMS[i]);

   }   
   int ind = 0;
  
    pos = strtok(NULL,":");
  
   if (pos!=NULL){
     while (pos!=NULL){
       if (strcmp(pos,"%7C") == 0){
           return true;      
       }
       strcpy(PARAMS[ind] , pos);    
       pos = strtok(NULL,":"); 
      
       ind++;
     }
   }
   return false;
}
