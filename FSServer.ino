/* 
  FSWebServer - Example WebServer with SPIFFS backend for esp8266
  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  upload the contents of the data folder with MkSPIFFS Tool ("ESP8266 Sketch Data Upload" in Tools menu in Arduino IDE)
  or you can upload the contents of a folder if you CD in that folder and run the following command:
  for file in `ls -A1`; do curl -F "file=@$PWD/$file" esp8266fs.local/edit; done
  
  access the sample web page at http://esp8266fs.local
  edit the page by going to http://esp8266fs.local/edit
*/
//#define USE_STA
#define USE_AP
#define USE_MP3
//#define USE_DHT
#define DBG_TO_FILE

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>

void logNTPTime();
void logFSInfo();

#ifdef DBG_TO_FILE
  File logger;
  #define DBG_OUTPUT_PORT if(logger.size()>10000){logger.close();logger=SPIFFS.open("/log","w");logger.println("cleared");logNTPTime();logFSInfo();}logger
#else
  #define DBG_OUTPUT_PORT Serial
#endif

#include "general_functions.h"
#include "filesystem_functions.h"
#include "net_functions.h"

#ifdef USE_DHT
  #include "dht_functions.h"
#endif
#ifdef USE_MP3
  #include "mp3_functions.h"
#endif

ESP8266WebServer server(80);

void setup(void){
  if(!SPIFFS.begin()) {
    Serial.begin(115200);
    Serial.println("SPIFFS mount failure");
    Serial.end();
  }
  #ifdef DBG_TO_FILE
    logger = SPIFFS.open("/log", "w");
  #else
    DBG_OUTPUT_PORT.begin(115200);
    DBG_OUTPUT_PORT.setDebugOutput(true);
  #endif
  DBG_OUTPUT_PORT.print("\nreset\n");

  logFSInfo();

  beginWiFi();
  
  //SERVER INIT
  //list directory
  server.on("/list", HTTP_GET, handleFileList);
  //load editor
  server.on("/edit", HTTP_GET, [](){
    if(!handleFileRead("/edit.htm")) server.send(404, "text/plain", "FileNotFound");
  });
  //create file
  server.on("/edit", HTTP_PUT, handleFileCreate);
  //delete file
  server.on("/edit", HTTP_DELETE, handleFileDelete);
  //first callback is called after the request has ended with all parsed arguments
  //second callback handles file uploads at that location
  server.on("/edit", HTTP_POST, [](){ server.send(200, "text/plain", ""); }, handleFileUpload);

  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  //get heap status, analog input value and all GPIO statuses in one json call
  server.on("/all", HTTP_GET, [](){
    String json = "{";
    json += "\"heap\":"+String(ESP.getFreeHeap());
    json += ", \"analog\":"+String(analogRead(A0));
    json += ", \"gpio\":"+String((uint32_t)(((GPI | GPO) & 0xFFFF) | ((GP16I & 0x01) << 16)));
    json += "}";
    server.send(200, "text/json", json);
    json = String();
  });

  #ifdef USE_DHT
    server.on("/dht", HTTP_GET, getDHTinfo);
    ticker.attach(60, flagDHT);
  #endif

  #ifdef USE_MP3
    mp3.begin();
    server.on("/play", HTTP_GET, mp3_play);
    server.on("/stop", HTTP_GET, mp3_stop);
  #endif
  
  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");
  logNTPTime();
}

void loop(void){
  server.handleClient();
  ONLOOP
}
