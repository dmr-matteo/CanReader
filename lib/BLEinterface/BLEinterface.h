#ifndef BLE_INTERFACE_H
#define BLE_INTERFACE_H

#include <BluetoothSerial.h>
#include <MyDisplay.h>
#include <FreeRTOS.h>
#include <task.h>

class BLEinterface {
protected:
    const char* deviceName;
    MyDisplay* display;
    BluetoothSerial btSerial;
    void (*dataCallback)(const char*, size_t); // Callback for received data

public:
    BLEinterface(const char* deviceName, MyDisplay* display);
    bool BluetoothInit();
    bool displayInfo();
    bool sendTo(const char* data);
    void onDataReceived(void (*callback)(const char* data, size_t length));
    void checkAndRespond(const char* data, size_t length);
    void processReceivedData();
    static void pingTestTask(void* pvParameters);
    void PingTest();
};

#endif