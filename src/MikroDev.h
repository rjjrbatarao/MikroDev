#ifndef __MIKRO_DEV_H
#define __MIKRO_DEV_H
#include <Arduino.h>

#if defined(ESP8266)
	#include <SoftwareSerial.h>
	#define rx_pin 14 // D5
	#define tx_pin 12 // D6
	#define swSer SoftwareSerial
	#define hdSer HardwareSerial
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#include <SoftwareSerial.h>
	#define swSer SoftwareSerial
	#define hdSer HardwareSerial
#elif defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__)
	#include <SoftwareSerial.h>
	#define swSer SoftwareSerial
	#define hdSer HardwareSerial
#elif defined(ESP32)
	#define hdSer HardwareSerial
#else
	#define hdSer HardwareSerial
	#define STM32
#endif

#define EVENT_0 1
#define EVENT_1 2
#define EVENT_2 3
#define EVENT_3 4
#define EVENT_4 5

//#define RECEIVE_DONE "DONE"
//#define RECEIVE_FAIL "FAIL"
//#define RECEIVE_READY "READY"
#define NUMBER_EVENTS 5

#define BAUD_RATE 9600
#define RX_BUFFER 48
#define RESPONSE_SIZE 128
//#define DEBUG_MODE
#define EV_0 "ev_0"
#define EV_1 "ev_1"
#define EV_2 "ev_2"
#define EV_3 "ev_3"
#define EV_4 "ev_4"

class mikrodev {
	
	public:
	mikrodev(hdSer& hardser);
	#ifndef STM32
	#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	mikrodev(swSer& sofser);
	#endif
	#endif
	~mikrodev();
	#ifndef STM32
	#if defined(ESP8266)
	void init(long baud = BAUD_RATE ,int rx = rx_pin, int tx = tx_pin);	
	#elif defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	void init(long baud = BAUD_RATE);	
	#endif
	#else
	void init(long baud = BAUD_RATE);		
	#endif
	void sendCommand(char* command);
	void sendCommand(const __FlashStringHelper* command);
	void sendCommand(char* command, char* arg_name, char(&response)[RX_BUFFER]);
	void sendCommand(const __FlashStringHelper* command, char* arg_name, char(&response)[RX_BUFFER]);
    void event(int& type, char(&response)[RESPONSE_SIZE], char* events_array[NUMBER_EVENTS]);
	void event(char * event_name, void (*f)(char*));
	void update();
	void clearBuffer();
	
	private:
	bool _isHardSerial = false;
	#ifndef STM32
	#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	swSer* _sofser;
	#endif
	#endif
	hdSer* _hardser;
	char _buffer[RESPONSE_SIZE];
	uint8_t _event_type;
	char _data_rx[RX_BUFFER];
	char _response[RESPONSE_SIZE];
	char _arg_name[RX_BUFFER];
	bool onReceive();
	bool onEventReceive();
	bool responseData(char* arg);
	const char* _RECEIVE_READY = "READY";
	char* _events_array[NUMBER_EVENTS];
	bool _event_flag = false;
    uint8_t _events_index;
    void (*events[NUMBER_EVENTS])(char*);
    char* events_names[NUMBER_EVENTS];

};

#endif