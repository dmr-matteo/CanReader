#include "MyNetTCP.h"

MyNetTCP::MyNetTCP(const char* ssid, const char* password, 
                                       IPAddress local_IP, IPAddress gateway, IPAddress subnet, MyDisplay* display) 
    : ssid(ssid), password(password), local_IP(local_IP), gateway(gateway), subnet(subnet), display(display), server(1234) {}

bool MyNetTCP::WifiInit() {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
        if (display) display->writeScreen("AP Config Failed");
        return false;
    }
    if (!WiFi.softAP(ssid, password, 1 , false)) {
        if (display) display->writeScreen("AP Start Failed");
        return false;
    }
    if (display) display->writeScreenWiFiStatus();
    return true;
}

bool MyNetTCP::displayInfo() {
    if (display) {
        display->writeScreen("IP: " + WiFi.softAPIP().toString());
        display->writeScreen(String(ssid) + "|" + String(password));
    }
    return true;
}

bool MyNetTCP::TCPbegin(uint16_t port) {
    this->port = port;
    server.begin();
    if (display) display->writeScreenNetworkProtocol("TCP ");
    return true;
}

void MyNetTCP::onDataReceived(void (*callback)(const char*, IPAddress, uint16_t)) {
    dataCallback = callback;
}

void MyNetTCP::checkAndRespond(const char* data, IPAddress remoteIP, uint16_t remotePort) {
    if (dataCallback) {
        dataCallback(data, remoteIP, remotePort);
    }
    if (strcmp(data, "ConnectionRequest") == 0) {
        client.println(data);
    } else if (strcmp(data, "PingTest") == 0) {
        PingTest(remoteIP, remotePort);
    }
}

void MyNetTCP::processReceivedData() {
    client = server.accept();  // Check if a new client is available
    if (client) {
        String incomingData = "";
        while (client.available()) {
            char c = client.read();  // Read byte from the client
            incomingData += c;
        }
        checkAndRespond(incomingData.c_str(), client.remoteIP(), client.remotePort());
    }
}

void MyNetTCP::PingTest(IPAddress remoteIP, uint16_t remotePort) {
    unsigned long startTime = millis();
    int i = 0;
    while (millis() - startTime < 3000) {
        String packet = String(i);
        client.println(packet);  // Send packet over the TCP connection
        i++;
        delay(10);
    }
    Serial.print("Ho inviato: ");
    Serial.println(i);
    Serial.print("Elaborati: ");
    Serial.println(float(i) / 3000.0);
}

void MyNetTCP::pingTestTask(void* pvParameters) {
    MyNetTCP* instance = static_cast<MyNetTCP*>(pvParameters);
    instance->PingTest(instance->client.remoteIP(), instance->client.remotePort());
    vTaskDelete(NULL);
}