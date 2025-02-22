#include "MyDisplay.h"

// Remove the constructor definition from here
// MyDisplay::MyDisplay(int width, int height, int address)
//     : display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE), row1("initialization..."), row2("SW version 1.0"), row3("HW version 1.0") {}

void MyDisplay::begin() {
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tr);
  display.drawStr(0, 10, "Hello World!");
  display.sendBuffer();
}

void MyDisplay::writeScreen(String stringa) {
    row3 = row2;
    row2 = row1;
    row1 = stringa;
    display.clearBuffer();
    display.setCursor(0, 8);
    display.println(row1.c_str());
    display.println(row2.c_str());
    display.println(row3.c_str());
    display.sendBuffer();
}

void MyDisplay::writeScreenHello(String stringa, int delayTime) {
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
    display.setCursor(0, 0);
    display.println(stringa.c_str());
    display.sendBuffer();
    delay(delayTime);
    display.clearBuffer();
    display.setFont(u8g2_font_ncenB08_tr);
}

void MyDisplay::writeScreenWiFiStatus() {
    display.setCursor(98, 0);
    display.print("AP ");
    display.drawBitmap(115, 0, 1, 8, WIFIcharIcone);
    display.sendBuffer();
}

void MyDisplay::writeScreenNetworkProtocol(String protocol) {
    display.setCursor(73, 0);
    display.print(protocol.c_str());
    display.sendBuffer();
}

void MyDisplay::writeScreenBenchmarkResults() {
    display.clearBuffer();
    display.setCursor(0, 8);
    display.print("Benchmark complete");
    display.sendBuffer();
}
