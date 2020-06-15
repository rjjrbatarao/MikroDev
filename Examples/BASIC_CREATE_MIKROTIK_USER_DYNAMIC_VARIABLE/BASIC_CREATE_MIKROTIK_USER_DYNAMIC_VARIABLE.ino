/*
   developed by: rjjrdev
   used for esp shield
   
   root commands: config, wifi, event, type, server, rest ,connect, array, task and api

   config mode1 mode2 mode3 mode4 mode5
   mode1: -d -r or -o
   mode2: -s or -o , -s = api-ssl enabled
   mode3: -o or key , key is the signature when api-ssl is enabled
   mode4: -a or -b , auto and basic, auto is rest api capable
   mode5: -rd enables the rest json response to be independently modified else if left blank then
   all keys are shared among the events
   note: if -rd is enabled additional second parameter is used on rest and type to identify its event_id
   note: array command is enabled when -rd is defined
   
   wifi sta_ssid sta_password/mode1 ap_ssid/mode2 ap_password/mode3 wireless_mode phy_mode tx_power
   sta_ssid: when set to -o enables smartConfig else sets the ssid to connect
   mode1&2&3: -o for blank
   wireless_mode: -a -s -as
   phy_mode: -b -g -n
   tx_power: -l and -h

   event event_name1 event_name2 event_name3 event_name4 event_name5
   note: default event name are event0 ... event4, you can use this to override event names
   note: maximum of 5 events you can use
   
   type num1 num2 num3 ...
   note: this only applies to config -o -o -o -a
   num1&2&3..10: 1 or 0 only to change key variables types to int or words respectively
   note: this corresponds to server keys count, if num1-num3 then key1-key3
   
   type event_id num1 num2 num3 ...
   note: this only applies to config -o -o -o -a -rd
   
   server port key1 key2 key3 ... key10
   note: when config ... -rd enable keys can be omitter therefor server port is used
   port: rest server port
   key1&2&3..10: 
   
   rest var0 var1 var2 var3 ... var10
   note: this only applies to config -d -o -o -a 
   var0&1&2&3..10: overridable variable key and value pair, check type how to change the data type
   
   rest event_id var1 var2 var3 ... var10
   note: this only applies to config -d -o -o -a -rd specify which event to modify
   var0&1&2&3..10: overridable variable key and value pair, check type how to change the data type
   
   array event_id key_1_1 key_1_1 key_1_1 ...key_1_1
   note: this is enabled when reponse dimension -rd is enabled
   note: this replaces command server key1 key2 key3...... thats why command server port is only used to initialize
   
   task:
   task_id: 0 for check for wifi link status, 1 for code generator, 2 for wifi info ip, device_id and rssi
   
   task 0 name mode
   name: -o to use default name as link
   mode: -ss for wifi status

   task 1 name mode1 mode2
   name: -o to use default name as link
   mode1: -l if lower case or -u if upper case
   mode2: length of code to generate if blank default of 8 is generated
   
   task 2 name mode
   mode -gt to get board info
   
   connect username password/mode port
   mode: -o for blank password
    
   api arg1 arg2 arg3
   arg1&2&3... mikrotik api commands
   consult mikrotik api for references
   this uses fully compatible mikrotik api
*/

#include <MikroDev.h>
#if defined(ESP8266) || defined(ESP32)
#include <SoftwareSerial.h>
SoftwareSerial softSer;
#else
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>
#include <MemoryFree.h>
SoftwareSerial softSer(10, 11);
#endif

mikrodev dev(softSer);

void createCommand() {
  char command_buffer[128];
  char* username = "test67";
  char* comment = "test_comment";
  sprintf(command_buffer, "api /ip/hotspot/user/set =numbers=%s =comment=%s", username, comment );
  dev.sendCommand(command_buffer);
}

void initialization() {
  dev.sendCommand("config -o");
  dev.sendCommand("wifi WIFISSID -o -o -o -s");
  dev.sendCommand("connect test -o 8728");
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
    }
  }
}
