/*
   developed by: rjjrdev
   used for esp shield
   
   root commands: config, wifi, event, type, server, rest ,connect and api

   config mode1 mode2 mode3 mode4
   mode1: -d -r or -o
   mode2: -s or -o
   mode3: -o or key
   mode4: -a or -b

   wifi sta_ssid sta_password/mode1 ap_ssid/mode2 ap_password/mode3 wireless_mode phy_mode tx_power
   mode1&2&3: -o for blank
   wireless_mode: -a -s -as
   phy_mode: -b -g -n
   tx_power: -l and -h

   event event_name1 event_name2 event_name3 event_name4 event_name5
   note: default event name are event0 ... event4, you can use this to override event names
   note: there are only 5 event you can use
   
   type num1 num2 num3 ...
   num1&2&3..10: 1 or 0 only to change key variables types to int or words respectively
   note: this corresponds to server keys count, if num1-num3 then key1-key3

   server port key1 key2 key3 ... key10
   port: rest server port
   key1&2&3..10: 
   
   rest response var1 var2 var3 ... var10
   response: special key in integer cannot be overriden using the type command
   var1&2&3..10: overridable variable key and value pair, check type how to change the data type
   
   connect username password/mode port
   mode: -o for blank password
    
   api arg1 arg2 arg3
   arg1&2&3... mikrotik api commands
   consult mikrotik api for references
   this uses standard mikrotik api fully compatible
*/

#include <MikroDev.h>
#include <SoftwareSerial.h>
#if defined(ESP8266)
SoftwareSerial softSer;
#else
#include <avr/pgmspace.h>
#include <MemoryFree.h>
#include <avr/wdt.h>
SoftwareSerial softSer(6, 7);
#endif

#define EVENT_1 1
#define EVENT_2 2
#define EVENT_3 3
#define EVENT_4 4
#define EVENT_5 5

mikrodev dev(softSer);

void sendRestData(){
  dev.sendCommand(F("rest 0 73 23 12"));
  dev.sendCommand(F("rest 1 32 54 23 90"));
  dev.sendCommand(F("rest 2 76 87"));
}
void sendReboot(){
  dev.sendCommand(F("config -r"));
}
/*
 *  |ev1|ev2|ev3|ev4|
 *   ----------------
 *   a_1|b_1|c_1|d_1
 *   a_2|b_2|c_2|d_2
 *   a_3|b_3|   |d_3
 *      |b_4|
 */     
void initialization() {
  dev.sendCommand(F("config -r"));
  delay(500);
  dev.sendCommand(F("config -o -o -o -a -rd"));
  dev.sendCommand(F("wifi WIFISSID -o -o -o -s -g -h"));
  dev.sendCommand(F("event dir1 dir2 dir3"));
  dev.sendCommand(F("verify 1 0 0"));
  dev.sendCommand(F("verifier -o 8 0 1 0"));        
  dev.sendCommand(F("array 0 a_1 a_2 a_3"));  
  dev.sendCommand(F("array 1 b_1 b_2 b_3 b_4")); 
  dev.sendCommand(F("array 2 c_1 c_2")); 
  dev.sendCommand(F("server 80"));
}

void eventFirst(char* response) {
  Serial.println(response);
}

void eventSecond(char* response){
  Serial.println(response);
}

void eventThird(char* response){
  Serial.println(response);
}

void eventFourth(char* response){
  Serial.println(response);
}

void eventFifth(char* response){
  Serial.println(response);
}
void setup() {
  Serial.begin(9600);
  dev.init(9600);// gpio 5 and 6 rx tx
  initialization();
  //       REST EVENTS EXTERNAL MAX 5
  dev.event("dir1:", eventFirst);
  dev.event("dir2:", eventSecond);
  dev.event("dir3:", eventThird);
  Serial.println("Initialization Finished");
}

void freeMem() {
#if defined(ESP8266)
  Serial.printf("[HEAP] Mem Free: %d\n", free);
  Serial.printf("[HEAP] Free Heap: %d\n", ESP.getFreeHeap());
  Serial.printf("[HEAP] Fragmentation: %d\n", ESP.getHeapFragmentation());
  Serial.printf("[HEAP] Max Free Heap: %d\n", ESP.getMaxFreeBlockSize());
#else
  Serial.println(freeMemory());
#endif
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 'c') {
      sendRestData();
      freeMem();
    } else if(c == 'r'){
      sendReboot();
      freeMem();
    }
  }  
  dev.update();
}
