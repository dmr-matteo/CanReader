#include "MyNetTCP.h"

MyNetTCP::MyNetTCP(const char* ssid, const char* password, 
                                       IPAddress local_IP, IPAddress gateway, IPAddress subnet, MyDisplay* display) 
    : ssid(ssid), password(password), local_IP(local_IP), gateway(gateway), subnet(subnet), display(display), server(1234) {}
bool MyNetTCP::WifiInit() { // Starting AP wifi server
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
        //if (display->isEnable) display->writeScreen("AP Config Failed");
        return false;
    }
    if (!WiFi.softAP(ssid, password, 1 , false)) {
        //if (display->isEnable) display->writeScreen("AP Start Failed");
        return false;
    }
    if (display->isEnable) display->writeScreenWiFiStatus();
    return true;
}

bool MyNetTCP::displayInfo() {
    if (display->isEnable) {
        display->writeScreen("IP: " + WiFi.softAPIP().toString());
        display->writeScreen(String(ssid) + "|" + String(password));
    }
    return true;
}

bool MyNetTCP::TCPbegin(uint16_t port) {
    this->port = port;
    server.begin();
    if (display->isEnable) display->writeScreenNetworkProtocol("TCP ");
    return true;
}

void MyNetTCP::onDataReceived(void (*callback)(const char*, IPAddress, uint16_t)) {
    dataCallback = callback;
}

void MyNetTCP::checkAndRespond(const char* data, IPAddress remoteIP, uint16_t remotePort) {
    if (dataCallback) {
        dataCallback(data, remoteIP, remotePort);
    }
    Serial.println(data);
    if (strcmp(data, "ConnectionRequest\n") == 0) {
        Serial.println("client connesso:)");
        client.println("connessione OK\n");
    } else if (strcmp(data, "PingTest") == 0) {
        PingTest(remoteIP, remotePort);
    }
}

void MyNetTCP::pingTestTask(void* pvParameters) {
    MyNetTCP* instance = static_cast<MyNetTCP*>(pvParameters);
    if (instance == nullptr) {
        Serial.println("Error: instance is null in pingTestTask");
        vTaskDelete(NULL);
        return;
    }
    if (!instance->client) {
        Serial.println("Error: client is not connected in pingTestTask");
        vTaskDelete(NULL);
        return;
    }
    instance->PingTest(instance->client.remoteIP(), instance->client.remotePort());
    vTaskDelete(NULL);
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
        Serial.println("No client connected");
    } else {
        
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

void MyNetTCP::onDataReceivedCallback(const char* data, IPAddress remoteIP, uint16_t remotePort) {
    Serial.print("Dati ricevuti da ");
    Serial.print(remoteIP);
    Serial.print(":");
    Serial.println(remotePort);
    Serial.println(data);
  
    // Rispondi al client (se necessario)
    if (strcmp(data, "PingTest") == 0) {
      Serial.println("PingTest ricevuto, avvio PingTest...");
    } else if (strcmp(data, "ConnectionRequest") == 0) {
      Serial.println("Richiesta di connessione ricevuta!");
    }
  }
