#include "BLEinterface.h"

BLEinterface::BLEinterface(const char* deviceName, MyDisplay* display)
    : deviceName(deviceName), display(display) {}

bool BLEinterface::BluetoothInit() {
    if (!btSerial.begin(deviceName)) {
        if (display) display->writeScreen("BT Init Failed");
        return false;
    }
    if (display) display->writeScreen("BT Initialized");
    return true;
}

bool BLEinterface::displayInfo() {
    if (display) {
        display->writeScreen("Device: " + String(deviceName));
    }
    return true;
}

bool BLEinterface::sendTo(const char* data) {
    btSerial.write(reinterpret_cast<const uint8_t*>(data), strlen(data));
    return true;
}

void BLEinterface::onDataReceived(void (*callback)(const char* data, size_t length)) {
    dataCallback = callback;
}

void BLEinterface::PingTest() {
    unsigned long startTime = millis();
    int i = 0;
    byte packet2[160] = {0x01};
    while (millis() - startTime < 3000) {
        packet2[1] = (i >> 24) & 0xFF;
        packet2[2] = (i >> 16) & 0xFF;
        packet2[3] = (i >> 8) & 0xFF;
        packet2[4] = i & 0xFF;
        sendTo(reinterpret_cast<const char*>(packet2));
        i++;
        delayMicroseconds(10);
    }
    Serial.print("Ho inviato: ");
    Serial.println(i);
    Serial.print("Elaborati: ");
    Serial.print(float(i) * sizeof(packet2) * 8.0 / 3000000.0);
    Serial.println("Mbit/s");
}

void BLEinterface::pingTestTask(void* pvParameters) {
    BLEinterface* instance = static_cast<BLEinterface*>(pvParameters);
    instance->PingTest();
    vTaskDelete(NULL);
}

void BLEinterface::checkAndRespond(const char* data, size_t length) {
    if (dataCallback) {
        dataCallback(data, length);
    }
    if (strcmp(data, "ConnectionRequest") == 0) {
        sendTo("ConnectionOK");
    } else if (strcmp(data, "PingTest") == 0) {
        PingTest();
       // xTaskCreate(pingTestTask, "PingTestTask", 10000, this, 1, NULL);  // 10k stack, priority 1
    }
}

void BLEinterface::processReceivedData() {
    if (btSerial.available()) {
        char incomingData[255];
        size_t len = btSerial.readBytes(incomingData, 255);
        if (len > 0) {
            incomingData[len] = '\0'; // Ensure null-terminated data
            checkAndRespond(incomingData, len);
        }
    }
}