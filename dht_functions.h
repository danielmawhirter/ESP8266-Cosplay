#pragma once
#include <dht11.h>

#define DHT11PIN D4
Ticker ticker;
float temperature = 0, humidity = 0;
bool updateNow = false;
dht11 DHT11;

void flagDHT() {
  updateNow = true;
}

void updateDHT() {
  int result = DHT11.read(DHT11PIN);
  if(DHTLIB_OK != result) {
    yield();
    result = DHT11.read(DHT11PIN);
    if(DHTLIB_ERROR_TIMEOUT == result) {
      DBG_OUTPUT_PORT.println("DHT error: timeout");
    } else if(DHTLIB_ERROR_CHECKSUM == result) {
      DBG_OUTPUT_PORT.println("DHT error: checksum");
    }
  }
}

void getDHTinfo() {
  String response = "Humidity (%): ";
  response += DHT11.humidity;
  response += "\nTemperature (C): ";
  response += DHT11.temperature;
  server.send(200, "text/plain", response);
  DBG_OUTPUT_PORT.println("DHT data pull");
}

#define ONLOOP if(updateNow){updateNow = false;updateDHT();}
