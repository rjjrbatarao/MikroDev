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
#if defined(ESP8266) || defined(ESP32)
SoftwareSerial softSer;
#else
#include <avr/pgmspace.h>
#include <MemoryFree.h>
SoftwareSerial softSer(10, 11);
#endif

mikrodev dev(softSer);


const char modify[] PROGMEM = {"api /ip/hotspot/user/set =numbers=test23 =comment=test45"};
const char print[] PROGMEM = {"api /ip/hotspot/user/print ?name=test23 =.proplist=limit-uptime"};
const char create[] PROGMEM = {"api /ip/hotspot/user/add =name=test23 =comment=test23 =limit-uptime=10h"};

const char arg_0[] PROGMEM = {"=name="};
const char arg_1[] PROGMEM = {"=comment="};
const char arg_2[] PROGMEM = {"=limit-uptime="};

const char init_0[] PROGMEM = {"config -o"};
const char init_1[] PROGMEM = {"wifi PISOWIFI -o -o -o -s"};
const char init_2[] PROGMEM = {"connect test -o 8728"};

const char *const arg_table[] PROGMEM = {arg_0, arg_1, arg_2};
const char *const init_table[] PROGMEM = {init_0, init_1, init_2};
const char *const command_table[] PROGMEM = {create, modify, print};

char commandBuffer[96];
char argBuffer[32];
char initBuffer[32];

void getProgmem(int index) {
  memset(commandBuffer, 0, sizeof(commandBuffer));
  memset(argBuffer, 0, sizeof(argBuffer));
  strcpy_P(commandBuffer, (char *)pgm_read_word(&(command_table[index])));
  strcpy_P(argBuffer, (char *)pgm_read_word(&(arg_table[index])));
}

void getInitProgmem(int index){
  memset(initBuffer, 0, sizeof(initBuffer));
  strcpy_P(initBuffer, (char *)pgm_read_word(&(init_table[index])));  
}

void createCommand() {
  getProgmem(0);
  char reponseData[rx_buffer];
  dev.sendCommand(commandBuffer, argBuffer, reponseData);
  Serial.println(reponseData);
}

void modifyCommand() {
  getProgmem(1);
  char reponseData[rx_buffer];
  dev.sendCommand(commandBuffer, argBuffer, reponseData);
  Serial.println(reponseData);
}

void printCommand() {
  getProgmem(2);
  char reponseData[rx_buffer];
  dev.sendCommand(commandBuffer, argBuffer, reponseData);
  Serial.println(reponseData);
}

void initialization() {
  getInitProgmem(0);
  dev.sendCommand(initBuffer);
  getInitProgmem(1);
  dev.sendCommand(initBuffer);
  getInitProgmem(2);
  dev.sendCommand(initBuffer);
}

void setup() {
  Serial.begin(9600);
  dev.init(9600);
  initialization();
  Serial.println("start");
}

void freeMem() {
  #if defined(ESP8266) || defined(ESP32)
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
      createCommand();
      freeMem();
    } else if (c == 'm') {
      modifyCommand();
      freeMem();
    } else if (c == 'p') {
      printCommand();
      freeMem();
    }
  }
}
