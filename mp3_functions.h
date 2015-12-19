#pragma once
#include <SerialMP3ESP.h>

SerialMP3 mp3;

void mp3_play() {
  mp3.play(1, 1);
  server.send(200, "text/plain", "playing");
  DBG_OUTPUT_PORT.println("MP3: play");
}

void mp3_stop() {
  mp3.stop();
  server.send(200, "text/plain", "stopped");
  DBG_OUTPUT_PORT.println("MP3: stop");
}

#define ONLOOP //
