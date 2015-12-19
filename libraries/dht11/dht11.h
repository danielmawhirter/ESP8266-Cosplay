
// http://playground.arduino.cc/Main/DHT11Lib

#ifndef dht11_h
#define dht11_h

#include <Arduino.h>

#define DHT11LIB_VERSION "0.4.1"

#define DHTLIB_OK				0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2

class dht11
{
public:
  int read(int pin);
	int humidity;
	int temperature;
};
#endif
