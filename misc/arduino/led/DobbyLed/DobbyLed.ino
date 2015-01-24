
#include <SPI.h>
#include <Ethernet.h>

// Ethernet Configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,10);
EthernetServer server(80);


#define PARAMSSIZE 10
String PARAMS[PARAMSSIZE];  
String METHOD;
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
}

void act(){
  
  // METHOD - name of method
  // PARAMS - array of params
  
  if (METHOD == "setLed"){
  
    if (PARAMS[0] == "1"){
      
      LED1REDTARGET = PARAMS[1].toInt();
      LED1GREENTARGET = PARAMS[2].toInt();
      LED1BLUETARGET = PARAMS[3].toInt();

      LED1SMOOTH = PARAMS[4].toInt();
      if (LED1SMOOTH==0){
       LED1SMOOTH = 1; 
      }

      // analogWrite(LED1REDPIN, PARAMS[1].toInt());
      // analogWrite(LED1GREENPIN, PARAMS[2].toInt());
      // analogWrite(LED1BLUEPIN, PARAMS[3].toInt());    
    }
     if (PARAMS[0] == "2"){
        LED2REDTARGET = PARAMS[1].toInt();
        LED2GREENTARGET = PARAMS[2].toInt();
        LED2BLUETARGET = PARAMS[3].toInt();
      
        LED2SMOOTH = PARAMS[4].toInt();
        if (LED2SMOOTH==0){
         LED2SMOOTH = 1; 
        } 
    }
    
     if (PARAMS[0] == "3"){
        LED3TARGET = PARAMS[1].toInt();
        LED3SMOOTH = PARAMS[2].toInt();
        if (LED3SMOOTH==0){
         LED3SMOOTH = 1; 
        } 
     }
     
     if (PARAMS[0] == "4"){
        LED4TARGET = PARAMS[1].toInt();
        LED4SMOOTH = PARAMS[2].toInt();
        if (LED4SMOOTH==0){
         LED4SMOOTH = 1; 
        } 
     }
     
     if (PARAMS[0] == "5"){
        LED5TARGET = PARAMS[1].toInt();
        LED5SMOOTH = PARAMS[2].toInt();
        if (LED5SMOOTH==0){
         LED5SMOOTH = 1; 
        } 
     }
     
     if (PARAMS[0] == "6"){
        LED6TARGET = PARAMS[1].toInt();
        LED6SMOOTH = PARAMS[2].toInt();
        if (LED6SMOOTH==0){
         LED6SMOOTH = 1; 
        } 
     }
     
     if (PARAMS[0] == "7"){
        LED7TARGET = PARAMS[1].toInt();
        LED7SMOOTH = PARAMS[2].toInt();
        if (LED7SMOOTH==0){
         LED7SMOOTH = 1; 
        } 
     }
     if (PARAMS[0] == "8"){
        LED8TARGET = PARAMS[1].toInt();
        LED8SMOOTH = PARAMS[2].toInt();
        if (LED8SMOOTH==0){
         LED8SMOOTH = 1; 
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
     
    
     String request;
     
     boolean currentLineIsBlank = true;
     while (client.connected()) {
      
     if (client.available()) {
        char c = client.read();               
        
        if (c == '\n' && currentLineIsBlank) {
           request = getRequest(request);
           sendHeaders(client);          
           parseRequest(request);          
           act();       
                      
          break;
        }
        if (c == '\n') {
        
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          request += c;
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

void parseRequest(String request){

   int pos = request.indexOf(':');
   if (pos==-1){
     METHOD = request;
   }else {
     METHOD = request.substring(0, pos); 
   }
  
  
   for (int i=0; i<PARAMSSIZE; i++){
     PARAMS[i] = "";
   }
   
   Serial.print("METHOD=");
   Serial.println(METHOD);
   request = request.substring(METHOD.length()+ 1 );
   
   int ind = 0;
   if (request.indexOf(':')!=-1){
     while (request.indexOf(':')!=-1){
       PARAMS[ind] = request.substring(0, request.indexOf(':'));
       request = request.substring(request.indexOf(':')+ 1 );
          
       Serial.print("PARAMS[");
       Serial.print(ind);
       Serial.print("]=");
       Serial.println(PARAMS[ind]);
       ind++;
     }
     PARAMS[ind] = request.substring(0, request.indexOf(':'));
     Serial.print("PARAMS[");
     Serial.print(ind);
     Serial.print("]=");
     Serial.println(PARAMS[ind]);
   }
  
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
