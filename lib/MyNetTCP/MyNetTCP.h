#ifndef MYNETTCP_H
#define MYNETTCP_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <MyDisplay.h>

class MyNetTCP {
protected:
    const char* ssid;
    const char* password;
    IPAddress local_IP;
    IPAddress gateway;
    IPAddress subnet;
    MyDisplay* display;
    uint16_t port;
    WiFiServer server;               // TCP server
    WiFiClient client;               // TCP client connection
    void (*dataCallback)(const char*, IPAddress, uint16_t); // Callback per i dati ricevuti

public:
    MyNetTCP(const char* ssid, const char* password, 
                       IPAddress local_IP, IPAddress gateway, IPAddress subnet, MyDisplay* display);
    
    bool WifiInit();
    bool displayInfo();
    bool TCPbegin(uint16_t port = 1234);

    void onDataReceived(void (*callback)(const char* data, IPAddress remoteIP, uint16_t remotePort));
    void checkAndRespond(const char* data, IPAddress remoteIP, uint16_t remotePort);
    void processReceivedData();

    void PingTest(IPAddress remoteIP, uint16_t remotePort);
    static void pingTestTask(void* pvParameters);
};

#endif
