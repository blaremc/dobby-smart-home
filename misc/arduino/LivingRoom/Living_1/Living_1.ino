
#include <IRremote.h>
#include <SPI.h>
#include <Ethernet.h>

// Ethernet Configuration
byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
IPAddress ip(192,168,1,11);
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


// Storage for the recorded code
int codeType = -1; // The type of code
unsigned long codeValue; // The code value if not raw
unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state


EthernetClient client;

void setup(void) {
  Ethernet.begin(mac,ip);
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
      if (atoi(PARAMS[1])==0){
        digitalWrite(RELE1PIN, HIGH); 
      } else {
        digitalWrite(RELE1PIN, LOW); 
      }
    }
    if (atoi(PARAMS[0])==2){
      if (atoi(PARAMS[1])==0){
        digitalWrite(RELE2PIN, HIGH); 
      } else {
        digitalWrite(RELE2PIN, LOW); 
      }
    }
  }
  
  
  if (strcmp(METHOD, "getLight") == 0){  
    client.println(analogRead(LIGHTPIN));    
  }  
  if (strcmp(METHOD,  "getMotion")== 0){  
    client.println(MOTIONVALUE);    
  }
  
  if (strcmp(METHOD,  "getIR")== 0){  
    
    client.println(codeType);
    client.println(codeValue);
 /*
    for (int i =0; i<10; i++){
      client.print(IRCOMMANDS[i]);
      client.print(" ");
    } */    
  }
 /*
  if (strcmp(METHOD,  "setIR")== 0){  
    Serial.println("setIR");
    Serial.println(PARAMS[0]);
//    irsend.sendNEC(PARAMS[0].toInt(), 32);   
    Serial.println("Sending...");
    sendCode(0);
    Serial.println("OK");
      irrecv.resume(); 
  }
  */

  
  
  if (strcmp(METHOD, "setLed")== 0){
  
    if (PARAMS[0] == "1"){
      
      LED1REDTARGET = atoi(PARAMS[1]);
      LED1GREENTARGET = atoi(PARAMS[2]);
      LED1BLUETARGET = atoi(PARAMS[3]);

      LED1SMOOTH = atoi(PARAMS[4]);
      if (LED1SMOOTH==0){
       LED1SMOOTH = 1; 
      }
    
    }
     if (PARAMS[0] == "2"){
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


void sendCode(int repeat) {
  if (codeType == NEC) {
    if (repeat) {
      irsend.sendNEC(REPEAT, codeLen);
      Serial.println("Sent NEC repeat");
    } 
    else {
      irsend.sendNEC(codeValue, codeLen);
      Serial.print("Sent NEC ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == SONY) {
    irsend.sendSony(codeValue, codeLen);
    Serial.print("Sent Sony ");
    Serial.println(codeValue, HEX);
  } 
  else if (codeType == RC5 || codeType == RC6) {
    if (!repeat) {
      // Flip the toggle bit for a new button press
      toggle = 1 - toggle;
    }
    // Put the toggle bit into the code to send
    codeValue = codeValue & ~(1 << (codeLen - 1));
    codeValue = codeValue | (toggle << (codeLen - 1));
    if (codeType == RC5) {
      Serial.print("Sent RC5 ");
      Serial.println(codeValue, HEX);
      irsend.sendRC5(codeValue, codeLen);
    } 
    else {
      irsend.sendRC6(codeValue, codeLen);
      Serial.print("Sent RC6 ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    irsend.sendRaw(rawCodes, codeLen, 38);
    Serial.println("Sent raw");
  }
}

