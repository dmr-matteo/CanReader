#ifndef MY_NETWORK_INTERFACE_H
#define MY_NETWORK_INTERFACE_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <MyDisplay.h>
#include <FreeRTOS.h>
#include <task.h>


class MyNetworkInterface {
protected:
    const char* ssid;
    const char* password;
    IPAddress local_IP;
    IPAddress gateway;
    IPAddress subnet;
    MyDisplay* display;
    uint16_t port;
    WiFiUDP udp;    
    IPAddress clientIP;
    uint16_t clientPort;
    void (*dataCallback)(const char*, IPAddress, uint16_t); // Callback per i dati ricevuti

public:
    MyNetworkInterface(const char* ssid, const char* password, 
                       IPAddress local_IP, IPAddress gateway, IPAddress subnet, MyDisplay* display);
    
    bool broadcast(const char* data);
    bool sendTo(const char* data, IPAddress clientIP, uint16_t clientPort);
    void onDataReceived(void (*callback)(const char* data, IPAddress remoteIP, uint16_t remotePort));
    bool WifiInit();
    bool displayInfo();
    bool UDPbegin(uint16_t port = 1234);

    void checkAndRespond(const char* data, IPAddress remoteIP, uint16_t remotePort);
    void PingTest(IPAddress remoteIP, uint16_t remotePort);
    static void pingTestTask(void* pvParameters);

    void processReceivedData();
};

#endif