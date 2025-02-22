#ifndef MYDYSPLAY_H
#define MYDYSPLAY_H

#include <U8g2lib.h>
#include <Wire.h>

class MyDisplay {
private:
  U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display;
  String row1;
  String row2;
  String row3;

  byte WIFIcharIcone[8] = {
    0b00100, 0b00010, 0b01001, 0b10101,
    0b10101, 0b01001, 0b00010, 0b00100
  };

public:
  bool isEnable = false;
  MyDisplay(int width = 128, int height = 32, int address = 0x3C)
      : display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE), row1("initialization..."), row2("SW version 1.0"), row3("HW version 1.0") {}

  void begin();
  void writeScreen(String stringa);
  void writeScreenHello(String stringa, int delayTime = 500);
  void writeScreenWiFiStatus();
  void writeScreenNetworkProtocol(String protocol);
  void writeScreenBenchmarkResults();
};

#endif
