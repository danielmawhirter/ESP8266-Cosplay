#pragma once
#include <Arduino.h>
#define PLAY_COMMAND 0x7EFF060F000000EF
#define PAUSE_COMMAND 0x7EFF060E000000EF
#define RESUME_COMMAND 0x7EFF060D000000EF
#define VOLUME_COMMAND 0x7EFF0606000000EF
#define VOLUME_UP_COMMAND 0x7EFF0604000000EF
#define VOLUME_DOWN_COMMAND 0x7EFF0605000000EF
#define STOP_COMMAND 0x7EFF0616000000EF
//#define LOOP_COMMAND 0x7EFF0619000000EF
//#define END_LOOP_COMMAND 0x7EFF0619000001EF
class SerialMP3 {
  private:
    void sendCommand(uint64_t cmd) {
      uint8_t* addr = (uint8_t*)(&cmd);
      for(int i = 0; i < 8; i++) {
        Serial1.write(*(addr + 7 - i));
      }
    }
  public:
    SerialMP3() {}
    void begin() {
      Serial1.begin(9600);
    }
    void play(uint8_t file_dir, uint8_t file_num) {
      uint64_t mask = file_dir;
      mask = ((mask << 8) | file_num) << 8;
      sendCommand(PLAY_COMMAND | mask);
    }
    void volume(uint8_t vol) {
      uint64_t mask = vol;
      mask <<= 8;
      sendCommand(VOLUME_COMMAND | mask);
    }
    void volume_up() {
      sendCommand(VOLUME_UP_COMMAND);
    }
    void volume_down() {
      sendCommand(VOLUME_DOWN_COMMAND);
    }
    void stop() {
      sendCommand(PAUSE_COMMAND);
    }
    void resume() {
      sendCommand(RESUME_COMMAND);
    }
    /*void loop() {
      sendCommand(LOOP_COMMAND);
    }
    void stop_loop() {
      sendCommand(END_LOOP_COMMAND);
    }*/
};