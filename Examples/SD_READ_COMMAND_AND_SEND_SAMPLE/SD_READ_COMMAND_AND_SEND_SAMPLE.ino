
//rest 1 %s %s %s %s this command has the larges %s counts which is 4

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
#include <SPI.h>
#include "SdFat.h"
#define MAX_ARG_COUNTS 4

#define EVENT_1 1
#define EVENT_2 2
#define EVENT_3 3
#define EVENT_4 4
#define EVENT_5 5

mikrodev dev(softSer);


const int chipSelect = 4;
const char* command_file = "commands.txt";

SdFat sd;
File ceateFile;

char* dynamicCommand[MAX_ARG_COUNTS] = {"32", "12", "43", "2"};


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

void initialization() {
  for (int i = 0; i < 7; i++) {
    sendLine(i, command_file);
  }
}

void eventFirst(char* response) {
  Serial.println(response);
}
void eventSecond(char* response) {
  Serial.println(response);
}
void eventThird(char* response) {
  Serial.println(response);
}
void eventFour(char* response) {
  Serial.println(response);
}
void eventFive(char* response) {
  Serial.println(response);
}

void task0() {
  sendLine(7, command_file);
}

void task1() {
  sendLine(8, command_file);
}

void rest0() {
  dynamicCommand[0] = "87";
  dynamicCommand[1] = "23";
  dynamicCommand[2] = "43";
  sendLine(9, command_file);
}

void rest1() {
  dynamicCommand[0] = "3";
  dynamicCommand[1] = "43";
  dynamicCommand[2] = "16";
  dynamicCommand[3] = "17";
  sendLine(10, command_file);
}

void rest2() {
  dynamicCommand[0] = "63";
  dynamicCommand[1] = "56";
  sendLine(11, command_file);
}

bool sendLine(int index, const char* file) {
  SdFile readFile(file, O_RDONLY);
  if (!readFile.isOpen()) {
    return false;
  } else {
    size_t n;
    uint8_t ln = 0;
    bool sts = false;
    char line[64];
    while ((n = readFile.fgets(line, sizeof(line))) > 0) {
      if (ln == index) {
        sts = true;
        break;
      }
      if (line[n - 1] != '\n') {
        sts = false;
      }
      ln++;
    }
    if (sts) {
      int pn = 0;
      for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '%') {
          pn++;
        }
      }
      if (pn == 0) {
        Serial.println(line);
        dev.sendCommand(line);
      } else {
        char command_temp[64];
        sprintf(command_temp, line, dynamicCommand[0], dynamicCommand[1], dynamicCommand[2], dynamicCommand[3], dynamicCommand[4]);
        Serial.println(command_temp);
        dev.sendCommand(command_temp);
      }
    }
    return sts;
  }
}

void setup() {
  Serial.begin(9600);
  dev.init(9600);// gpio 5 and 6 rx tx

  // EXTERNAL EVENTS FROM REST MAX 5
  dev.event("ev1:", eventFirst);
  dev.event("ev2:", eventSecond);
  dev.event("ev3:", eventThird);

  // INTERNAL EVENTS FROM TASK MAX 5
  dev.event("code:", eventFour);
  dev.event("link:", eventFive);

  Serial.println("start");

  //*********************** SD INITS **************************

  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }
  
  //******************** INITIALIZATION COMMANDS **************
  initialization();
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '0') {
      task0();
      freeMem();
    } else if (c == '1') {
      task1();
      freeMem();
    } else if (c == 'a') {
      rest0();
      freeMem();
    } else if (c == 'b') {
      rest1();
      freeMem();
    } else if (c == 'c') {
      rest2();
      freeMem();
    }
  }
  dev.update();
}
