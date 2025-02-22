#include "MyNetworkInterface.h"

MyNetworkInterface::MyNetworkInterface(const char* ssid, const char* password, 
                                       IPAddress local_IP, IPAddress gateway, IPAddress subnet, MyDisplay* display) 
    : ssid(ssid), password(password), local_IP(local_IP), gateway(gateway), subnet(subnet), display(display) {}

bool MyNetworkInterface::WifiInit() {
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

bool MyNetworkInterface::displayInfo() {
    if (display) {
        display->writeScreen("IP: " + WiFi.softAPIP().toString());
        display->writeScreen(String(ssid) + "|" + String(password));
    }
    return true;
}

bool MyNetworkInterface::UDPbegin(uint16_t port) {
    this->port = port;
    if (!udp.begin(port)) {
        if (display) display->writeScreenNetworkProtocol("FFF ");
        return false;
    } else {
        if (display) display->writeScreenNetworkProtocol("UDC ");
    }
    return true;
}

bool MyNetworkInterface::broadcast(const char* data) {
    udp.beginPacket(IPAddress(255, 255, 255, 255), port);
    udp.write(reinterpret_cast<const uint8_t*>(data), strlen(data));
    return udp.endPacket();
}

bool MyNetworkInterface::sendTo(const char* data, IPAddress clientIP, uint16_t clientPort) {
    udp.beginPacket(clientIP, clientPort);
    udp.write(reinterpret_cast<const uint8_t*>(data), strlen(data));
    return udp.endPacket();
}

void MyNetworkInterface::onDataReceived(void (*callback)(const char*, IPAddress, uint16_t)) {
    dataCallback = callback;
}

void MyNetworkInterface::PingTest(IPAddress remoteIP, uint16_t remotePort) {
    unsigned long startTime = millis();
        int i = 0;
        byte packet2[160] = {0x01};
        while (millis() - startTime < 3000) {
            packet2[1] = (i >> 24) & 0xFF;
            packet2[2] = (i >> 16) & 0xFF;
            packet2[3] = (i >> 8) & 0xFF;
            packet2[4] = i & 0xFF;
            udp.beginPacket(remoteIP, 1235);
            udp.write(packet2, sizeof(packet2));
            udp.endPacket();
            i++;
            delayMicroseconds(10);
        }
        Serial.print("Ho inviato: ");
        Serial.println(i);
        Serial.print("Elaborati: ");
        Serial.print(float(i) * sizeof(packet2) * 8.0 / 3000000.0);
        Serial.println("Mbit/s");        
}
void MyNetworkInterface::pingTestTask(void* pvParameters) {
    MyNetworkInterface* instance = static_cast<MyNetworkInterface*>(pvParameters);
    IPAddress clientIP = instance->clientIP;
    uint16_t clientPort = instance->clientPort;
    instance->PingTest(clientIP, clientPort);
    vTaskDelete(NULL);
}


void MyNetworkInterface::checkAndRespond(const char* data, IPAddress remoteIP, uint16_t remotePort) {
    if (dataCallback) {
        dataCallback(data, remoteIP, remotePort);
    }
    if (strcmp(data, "ConnectionRequest") == 0) {
        clientIP = remoteIP;
        clientPort = remotePort;
        sendTo("ConnectionOK", remoteIP, remotePort);
    } else if (strcmp(data, "PingTest") == 0) {
        PingTest(clientIP, clientPort);
        //xTaskCreate(pingTestTask, "PingTestTask", 10000, this, 2, NULL);  // 10k di stack, priorità
        //PingTest(remoteIP, remotePort);
    }
}


void MyNetworkInterface::processReceivedData() {
    int packetSize = udp.parsePacket();
    if (packetSize) {
        // Ricevi i dati dal pacchetto
        char incomingData[255];
        int len = udp.read(incomingData, 255);
        if (len > 0) {
            incomingData[len] = '\0'; // Assicurati che i dati siano null terminati
        }
        
        Serial.println("Ricevuto pacchetto di ");
        // Chiamare checkAndRespond per rispondere ai messaggi
        checkAndRespond(incomingData, udp.remoteIP(), udp.remotePort());
    }
}
