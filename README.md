# MikroDev

This is a library are used to communicate with the included esp8266 mikro shield firmware.
This enables you to develop mikrotik web apps or any automation device for mikrotik, like for example,
router monitoring, external mikrotik schedulers, hotspot automations, and anything you can think of using arduino boards.
The mikrodev project aims to reduce program size on memory constrained mcu like the arduino by offloading the processing and communications to the slave.
This saves alot of memory on the master mcu so you can even make a much complex projects using only arduino uno.
Why MikroDev? because this is an mikrotik api for developers using microcontrollers.
This library is currently tested on Arduino uno, mega, esp8266 and stm32f103 bluepill.
This library is the master library which communicates on the included slave firmware.

### Todo
```
upload and update ota on slave and master(uno only) via slave internal webserver in ap mode
online auto update on slave and master(uno only) using root command
add server events, websocket if memory allows
add more interesting features from suggestions
```
### Current Features

Note: this features are the capability of MikroDev library to send and receive command to and from the slave firmware.

Basic version
```
> Full mikrotik api on standard user define port
> Wifi status and board status
> Esp8266 smartConfig
> Wifi phy B\G\N selectable
```

Unlocked version
```
> Full mikrotik api through unsecured user define port
> Full Api-SSL through user defined port and ssl key
> Code generator max length of 16 characters
> Total of five asynchronous timers
> Total of five unique REST api route using event command
> Total of ten json key value pair response per REST route
> Independent or shared json response on REST Path
> REST api route protection using token or source ip can be turned on on off during run time
> REST api route enable or disable during runtime
> REST api dynamic parameter response capable json encoded callback 
> Esp8266 smartConfig
> Wifi status and board status
> Wifi phy B\G\N selectable
> REST server port selectable
```
### Prerequisites

```
Arduino IDE
Arduino softwareserial library
Knowledge in arduino and c++ programming
Knowledge in html and javascript
```

### Installing
```
Simply download this repo and install on arduino ide
Upload the firmware included on bin folder, use freematics arduino builder to upload firmware
```
### Connection
Arduino tx -> esp rx
Arduino rx -> esp tx

## Sample 

Send mikrotik command with immediate response parser
```cpp
  char reponseData[64];
  dev.sendCommand(F("api /ip/hotspot/user/add =name=test23 =comment=test23 =limit-uptime=10h"), "=name=", reponseData);
  Serial.println(reponseData);
```

Send mikrotik command without expecting for response
```cpp
  char buffer[64] = "api /ip/hotspot/user/add =name=test23 =comment=test23 =limit-uptime=10h";
  dev.sendCommand(buffer);
```
Send mikrotik static command
```cpp
  dev.sendCommand(F("api /system/identity/print"));
```
Send mikrotik dynamic command
```cpp
  char* username = "user_test";
  char* comment = "api test";
  char* timelimit = 10m;
  char buffer[64] = "api /ip/hotspot/user/add =name=test23 =comment=test23 =limit-uptime=10h";
  sprintf(buffer,"api /ip/hotspot/user/add =name=%s =comment=%s =limit-uptime=%s",username,comment,timelimit);
  dev.sendCommand(buffer);
```
Send with response event, this is an alternative to using command with immediate respose

```cpp
void eventFirst(char* response) {
  Serial.println(response);
}
void setup() {
dev.event("ev1:", eventFirst);
}
void loop() {
dev.update();
}
```
Note: the dev.update() only needed if using events based response
Also F() macro are used to save ram.
Please check included samples for for learning

## Documentation

Root commands: 
```
config, wifi, event, type, server, verify, verifier, timer, eeprom, rest ,connect, array, task and api
```

Command usage:
```
  > config mode1 mode2 mode3 mode4 mode5
   mode1: -d -r or -o
   mode2: -s or -o , -s = api-ssl enabled
   mode3: -o or key , key is the signature when api-ssl is enabled
   mode4: -a or -b , auto and basic, auto is rest api capable
   mode5: -rd enables the rest json response to be independently modified else if left blank then
   all json response key and value are shared among the events
   note: if -rd is enabled additional second parameter is used on rest and type to identify its event_id
   note: array command is enabled when -rd is defined
   
  > wifi sta_ssid sta_password/mode1 ap_ssid/mode2 ap_password/mode3 wireless_mode phy_mode tx_power
   sta_ssid: when set to -o enables smartConfig else sets the ssid to connect
   mode1&2&3: -o for blank
   wireless_mode: -a -s -as
   phy_mode: -b -g -n
   tx_power: -l and -h

  > event event_name1 event_name2 event_name3 event_name4 event_name5
   note: default event name are event0 ... event4, you can use this to override event names
   note: maximum of 5 events you can use
   
  > type num1 num2 num3 ...
   note: this only applies to config -o -o -o -a
   num1&2&3..10: 1 or 0 only to change key variables types to int or words respectively
   note: this corresponds to server keys count, if num1-num3 then key1-key3
   
  > type event_id num1 num2 num3 ...
   note: this only applies to config -o -o -o -a -rd
   
  > server port key1 key2 key3 ... key10
   note: when config ... -rd enable keys can be omitter therefor server port is used
   port: rest server port
   key1&2&3..10: 
   
  > rest var0 var1 var2 var3 ... var10
   note: this only applies to config -d -o -o -a 
   var0&1&2&3..10: overridable variable key and value pair, check type how to change the data type
   callback: parameters passed to the api json formatted
   
  > rest event_id var1 var2 var3 ... var10
   note: this only applies to config -d -o -o -a -rd specify which event to modify
   var0&1&2&3..10: overridable variable key and value pair, check type how to change the data type
   callback: parameters passed to the api json formatted
   
  > array event_id key_1_1 key_1_1 key_1_1 ...key_1_1
   note: this is enabled when reponse dimension -rd is enabled
   note: this replaces command server key1 key2 key3...... thats why command server port is only used to initialize
   
  > task:
   task_id: 0 for check for wifi link status, 1 for code generator, 2 for wifi info ip, device_id and rssi

  > task 0 name mode
   name: -o to use default name as link
   mode: -ss for wifi status
   callback:
   
  > task 1 name mode1 mode2
   name: -o to use default name as link
   mode1: -l if lower case or -u if upper case
   mode2: length of code to generate if blank default of 8 is generated
   callback:
   
  > task 2 name mode
   mode -gt to get board info
   
  > timer timer_id timername/mode1 mode2 timeout increments
   timer_id: 0 - 5
   mode1: -o will revert to default timername tm_0 ...tm_5
   mode2: -d to delete and -e to enable
   timeout: timeout in seconds
   increments: how many times it will timeout ex timer 0 -o -e 1 60 this with 1 second timeout and 60 increments is 1 minute
   callback: time remaining in seconds and decending
   
  > eeprom mode data
   mode: -p put the data in eeprom, -g get the data from eeprom
   data: 256byte max length data
   note: no need to to add data when using -g ie. eeprom -g
   
  > connect username password/mode port
   mode: -o for blank password
   port: 8728 standard port or when modified can be any, 8729 api-ssl or any when modified in mikrotik router
    
  > api arg1 arg2 arg3
   arg1&2&3... mikrotik api commands
   consult mikrotik api for references
   this uses fully compatible mikrotik api
```
