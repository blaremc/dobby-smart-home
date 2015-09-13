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
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x10 };
IPAddress ip(192,168,1,10);
EthernetServer server(80);
EthernetClient client;

byte SERVER[] = { 192, 168, 1, 4 };
EthernetClient client_get;

#define REQUESTSIZE 1000

#define IRCOMMANDSSIZE 20
String IRACTIONS[IRCOMMANDSSIZE]; 

#define PARAMSSIZE 8
char PARAMS[PARAMSSIZE][10];  
char METHOD[15];
int iteration;

#define LED1REDPIN 2
#define LED1GREENPIN 3
#define LED1BLUEPIN 4

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



#define LED2REDPIN 5
#define LED2GREENPIN 6
#define LED2BLUEPIN 7




#define LIGHTPIN A1 // сенсор света
#define MOTIONPIN 22 // сенсор движения
//#define IRTPIN 7 // ИК передатчик
#define IRRPIN 29 // ИК приемник
#define RELE1PIN 23 // Реле 1
#define RELE2PIN 24 // Реле 2

IRrecv irrecv(IRRPIN);
decode_results results;
long IRCOMMANDS[10];
int codeType = -1; // The type of code
unsigned int rawCodes[RAWBUF]; // The durations if raw
unsigned long codeValue; // The code value if not raw

IRsend irsend;





byte MOTIONVALUE = 0;
int MOTIONTIME = 0;
int MOTIONDELAY = 1000;
long SENDTIME = 0;
long SENDDELAY = 5000;
long MINSENDDELAY = 5000;
long MAXSENDDELAY = 200000;

long IRGETDELAY = 200000;
long IRSENDTIME = 200000;

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
  irrecv.enableIRIn();
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
     
      if (strcmp(PARAMS[1],"t")==0 ){ 
       
        if (RELE1VALUE){
          digitalWrite(RELE1PIN, HIGH); 
          RELE1VALUE = 0;
        }else {
          digitalWrite(RELE1PIN, LOW); 
          RELE1VALUE = 1;
        }        
      } else if (atoi(PARAMS[1])==0){       
        digitalWrite(RELE1PIN, HIGH); 
        RELE1VALUE = 0;
      } else if (atoi(PARAMS[1])==1 ){           
        digitalWrite(RELE1PIN, LOW); 
        RELE1VALUE = 1;
      }
    }
    if (atoi(PARAMS[0])==2){      
       if (strcmp(PARAMS[1],"t")==0 ){ 
        if (RELE2VALUE){
          digitalWrite(RELE1PIN, HIGH); 
          RELE2VALUE = 0;
        }else {
          digitalWrite(RELE1PIN, LOW); 
          RELE2VALUE = 1;
        } 
      } else  if (atoi(PARAMS[1])==0){
        digitalWrite(RELE2PIN, HIGH); 
        RELE2VALUE = 0;
      } else if (atoi(PARAMS[1])==1 ){ 
        digitalWrite(RELE2PIN, LOW); 
        RELE2VALUE = 1;
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
  
  client.println("Bedroom Arduino");
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
    String result = getServerResponse(client);
    result = result.substring(result.indexOf("GET /") + 5, result.indexOf(" ", 5));         
    doCommands(result); 
    sendHeaders(client); 
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

void doCommands(String commands){
  Serial.print("doCommands ");
  Serial.println(commands);
  char request[REQUESTSIZE];
  commands.toCharArray(request, commands.length() + 1);
  bool again = false;
  bool res = true;
  while (res){   
   res = parseRequest(request, again);
   again = true;
   Serial.print("COMMAND = ");
   Serial.println(METHOD);
   for (int i=0; i<10; i++){
   Serial.print("PARAMS[");
   Serial.print(i);
   Serial.print("] = ");
   Serial.println(PARAMS[i]);
    
   }
   act();       
 }
  
}

String getServerResponse(EthernetClient client){
    
  byte reqInd = 0;
  boolean currentLineIsBlank = true;
  String req = String("");
  
  while (client.connected()) {
  
     if (client.available()) {
        char c = client.read();        
        req += c;        
      }
  }
  
  Serial.print("String ");
  Serial.println(req);
  return req;
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
     client_get.println("Host: 192.168.1.4");
     client_get.println("User-Agent: arduino-ethernet");
     client_get.println("Connection: close");
     client_get.println();
     //delay(1500);
     String result = getServerResponse(client_get);
     result = result.substring(result.indexOf("\r\n\r\n") + 4);
    
    int i = 0;
    int ind = 0;
    while (ind != -1 && i<= IRCOMMANDSSIZE){
      IRACTIONS[i] = result.substring(ind, result.indexOf("\n", ind));
      IRACTIONS[i].trim();
      Serial.print("IR ");
      Serial.println(IRACTIONS[i]);
      ind = result.indexOf("\n", ind);
      if (ind!=-1){
        ind++;
      }
      i++;
    }
    for (int j=i; j<IRCOMMANDSSIZE; j++){
      IRACTIONS[j] = "";
    }
     
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
   delay(2000);
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
   
   for (int i=0; i<PARAMSSIZE; i++){
     memset(PARAMS[i], 0, sizeof PARAMS[i]);

   }   
   int ind = 0;
  
   pos = strtok(NULL,":");
 
   if (pos!=NULL){
     while (pos!=NULL){
       if (strcmp(pos,"%7C") == 0 || strcmp(pos,"|") == 0){
           return true;      
       }
       strcpy(PARAMS[ind] , pos);
   
       pos = strtok(NULL,":");       
       ind++;
     
     }
   }
   return false;
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


    char tbuf[80];
    sprintf(tbuf, "%i_%lu",codeType,(int)codeValue);   
    Serial.println(tbuf);
    int res = checkIRCode(String(tbuf));
    if (res){
      sprintf(buf, "%sdevice[]=%s&value[]=%i_%lu_1&", buf, "ir",codeType,(int)codeValue);   
    }else{
      sprintf(buf, "%sdevice[]=%s&value[]=%i_%lu_0&", buf, "ir",codeType,(int)codeValue);   
    }
    isclearbuf = 0;
  }

}

int checkIRCode(String command){

  
  Serial.print("SEARCH COMMAND ");
  Serial.println(command);
  for (int i=0; i<IRCOMMANDSSIZE; i++){
    
     Serial.print("COMMAND ");
     Serial.print(IRACTIONS[i]);
     Serial.print(" RES ");
     Serial.println(IRACTIONS[i].indexOf(command));
        
      if (IRACTIONS[i].indexOf(command)!=-1){
        doIRCommand(IRACTIONS[i]);      
        return 1;  
      }    
  }     
  return 0;
}

void doIRCommand(String command){
  command =  command.substring(command.indexOf(" ") + 1);
  doCommands(command);
}

