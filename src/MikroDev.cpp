#include "MikroDev.h"

#ifndef STM32
#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
mikrodev::mikrodev(swSer& sofser) {
  _isHardSerial = false;
  _events_array[0] = EV_0;
  _events_array[1] = EV_1;
  _events_array[2] = EV_2;
  _events_array[3] = EV_3;
  _events_array[4] = EV_4;
  this->_sofser = &sofser;
}
#endif
#endif
mikrodev::mikrodev(hdSer& hardser) {
  _isHardSerial = true;
  _events_array[0] = EV_0;
  _events_array[1] = EV_1;
  _events_array[2] = EV_2;
  _events_array[3] = EV_3;
  _events_array[4] = EV_4;
  this->_hardser = &hardser;
}

mikrodev::~mikrodev() {

}
#ifndef STM32
#if defined(ESP8266)
void mikrodev::init(long baud, uint8_t rx, uint8_t tx) {
  if (!_isHardSerial) {
    this->_sofser->begin(baud, SWSERIAL_8N1, rx, tx, false, 256);
  } else {
    this->_hardser->begin(baud);
  }
}
#elif defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
void mikrodev::init(long baud) {
  if (!_isHardSerial) {
    this->_sofser->begin(baud);
	//Serial.println("s");
  } else {
    this->_hardser->begin(baud);
	//Serial.println("h");
  }
}
#else
void mikrodev::init(long baud) {
  this->_hardser->begin(baud);
}
#endif
#else
void mikrodev::init(long baud) {
  this->_hardser->begin(baud);
}
#endif


void mikrodev::event(char * event_name, void (*event)(char*)) {
  events_names[_events_index] = event_name;
  events[_events_index] = event;
  _event_flag = true;
  _events_index++;

}


void mikrodev::update() {
  onEventReceive();
}

void mikrodev::event(int& type, char* response, char* events_array[5]) {
  _event_type = 0;
  _event_flag = false;

  for (uint8_t i = 0; i < 5; i++) {
	  if(_events_array[i] != ""){
		_events_array[i] = (events_array[i] != 0) ? events_array[i] : _events_array[i];
		_events_index++;
	  }
  }
  onEventReceive();
  strcpy(response, _response);
  type = _event_type;
}

void mikrodev::sendCommand(const __FlashStringHelper* command) {
#ifndef STM32
#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  _isHardSerial ? this->_hardser->println(command) : this->_sofser->println(command);
#else
  this->_hardser->println(command);
#endif
#else
  this->_hardser->println(command);
#endif
}

void mikrodev::sendCommand(char* command) {
#ifndef STM32
#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  _isHardSerial ? this->_hardser->println(command) : this->_sofser->println(command);
#else
  this->_hardser->println(command);
#endif
#else
  this->_hardser->println(command);
#endif
}

void mikrodev::sendCommand(const __FlashStringHelper* command, char* arg_name, char* response) {
#ifndef STM32
#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  _isHardSerial ? this->_hardser->println(command) : this->_sofser->println(command);
#else
  this->_hardser->println(command);
#endif
#else
  this->_hardser->println(command);
#endif
  if (response[0] != '\0') {
	  memset(response,0,sizeof(response));
  }
  strcpy(response, onReceive(arg_name));
}

void mikrodev::sendCommand(char* command, char* arg_name, char* response) {
#ifndef STM32
#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  _isHardSerial ? this->_hardser->println(command) : this->_sofser->println(command);
#else
  this->_hardser->println(command);
#endif
#else
  this->_hardser->println(command);
#endif

  if (response[0] != '\0') {
	  memset(response,0,sizeof(response));
  }  
  strcpy(response, onReceive(arg_name));
}

bool mikrodev::onEventReceive() {
  bool status = false;
#ifndef STM32
#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  while ((_isHardSerial ? this->_hardser->available() : this->_sofser->available()) > 0) {
    strcpy(_buffer, _isHardSerial ? this->_hardser->readStringUntil('\n').c_str() : this->_sofser->readStringUntil('\n').c_str());
#else
  while (this->_hardser->available() > 0) {
    strcpy(_buffer, this->_hardser->readStringUntil('\n').c_str());
#endif
#else
  while (this->_hardser->available() > 0) {
    strcpy(_buffer, this->_hardser->readStringUntil('\n').c_str());
#endif

#ifdef DEBUG_MODE
    Serial.println(_buffer);
#endif
	if (strcmp(_buffer, _RECEIVE_READY) == 0) {
	  status = true;
	  //_isHardSerial ? _hardser->flush() : _sofser->flush();
      break;
    }
    delay(1);
    for (uint8_t i = 0; i < _events_index ; i++) {
      if (responseData(_event_flag ? events_names[i] : _events_array[i])) {
        _event_type = i + 1;
		events[i](_response);
		status = true;
      }
    }
  }
  return status;
}

char* mikrodev::onReceive(char* argn) {
  bool status = false;
  //char response[RX_BUFFER];
  //char _data_rx[RX_BUFFER];
#ifndef STM32
#if defined(ESP8266) || defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  while ((_isHardSerial ? this->_hardser->available() : this->_sofser->available()) > 0) {
	strcpy(_buffer, _isHardSerial ? this->_hardser->readStringUntil('\n').c_str() : this->_sofser->readStringUntil('\n').c_str());
#else
  while (this->_hardser->available() > 0) {
    strcpy(_buffer, this->_hardser->readStringUntil('\n').c_str());
#endif
#else
  while (this->_hardser->available() > 0) {
    strcpy(_buffer,  this->_hardser->readStringUntil('\n').c_str());
#endif
#ifdef DEBUG_MODE
    Serial.println(_buffer);
#endif
	if (strcmp(_buffer, _RECEIVE_DONE) == 0) {
		 Serial.println("done");
		 return _response;
		  //_isHardSerial ? _hardser->flush() : _sofser->flush();
		 break;
	}	
	delay(1);
	if (responseData(argn) && strlen(argn) > 0) {
	#ifdef DEBUG_MODE
		Serial.println(_response);
	#endif
	    Serial.println("match");
		return _response;
	}	
  }
}

bool mikrodev::responseData(char* arg) {
  uint8_t len = strlen(arg);
  char parse_buff[32];
  memcpy(parse_buff,_buffer,len);
  parse_buff[len] = '\0';
  if (strcmp(arg, parse_buff) == 0) {
	  uint8_t idx = 0;
    for (idx = len; idx != strlen(_buffer); idx++) {
      _response[idx - len] = _buffer[idx];
    }
	_response[idx - len] = '\0';
  } else {
    if (_response[0] != '\0' && _buffer[0] != '\0') {
      memset(_response, 0, sizeof(_response));
	  memset(_buffer, 0 , sizeof(_buffer));
    }
  }
  if (_response[0] != '\0') {
    return true;
  } else {
    return false;
  }
}


void mikrodev::clearBuffer() {

}



