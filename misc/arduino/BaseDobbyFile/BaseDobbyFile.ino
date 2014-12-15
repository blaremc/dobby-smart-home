
#include <SPI.h>
#include <Ethernet.h>

// Ethernet Configuration
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,3,2);
EthernetServer server(80);


#define PARAMSSIZE 10
String PARAMS[PARAMSSIZE];  
String METHOD;

void setup(void) {
  Ethernet.begin(mac,ip);
  server.begin();
  Serial.begin(9600);   
 
}

void act(){
  
  // METHOD - name of method
  // PARAMS - array of params
  
  
  
}




void loop(void) {
   
  EthernetClient client = server.available();
           
  if(client){
     
     Serial.println("");
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
