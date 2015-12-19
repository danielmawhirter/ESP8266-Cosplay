#pragma once

extern ESP8266WebServer server;

//format bytes
String formatBytes(size_t bytes){
  if (bytes < 1024) {
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)) {
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)) {
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

String getContentType(String filename) {
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void logFSInfo() {
  String logResult = "";
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    logResult += "FS File: " + fileName + ", size: " + formatBytes(fileSize) + "\n";
  }
  DBG_OUTPUT_PORT.print(logResult);
  DBG_OUTPUT_PORT.printf("\n");
}

void logNTPTime() {
  DBG_OUTPUT_PORT.print("time: ");
  HTTPClient http;
  http.begin("www.timeapi.org", 80, "/utc/now");
  int httpCode = http.GET();
  if(httpCode > 0 && httpCode <= 200) {
    DBG_OUTPUT_PORT.println(http.getString());
  } else {
    DBG_OUTPUT_PORT.println("failed");
  }
}

