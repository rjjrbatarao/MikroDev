#include <SPI.h>
#include "SdFat.h"

const int chipSelect = 4;
const char* command_file = "commands.txt";
File ceateFile;
SdFat sd;



bool storeSampleCommands() {
  ceateFile = sd.open(command_file, FILE_WRITE);
  if (ceateFile) {

    ceateFile.println(F("config -d -o -o -a -rd"));
    ceateFile.println(F("wifi PISOWIFI -o -o -o -s -g -h"));
    ceateFile.println(F("event ev1 ev2 ev3"));

    // json key
    ceateFile.println(F("array 0 a_1 a_2 a_3"));
    ceateFile.println(F("array 1 b_1 b_2 b_3 b_4"));
    ceateFile.println(F("array 2 c_1 c_2"));

    ceateFile.println(F("server 80"));

    ceateFile.println(F("task 0 -o -ss"));
    ceateFile.println(F("task 1 -o -u 9"));


    // json value
    ceateFile.println(F("rest 0 %s %s %s"));
    ceateFile.println(F("rest 1 %s %s %s %s"));
    ceateFile.println(F("rest 2 %s %s"));


    ceateFile.close();
    return true;
  } else {
    return false;
  }
}

void setup() {
  // put your setup code here, to run once:
  if (!sd.begin(chipSelect, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }

  sd.remove(command_file);

  storeSampleCommands();
}

void loop() {
  // put your main code here, to run repeatedly:

}
