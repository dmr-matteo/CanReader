/*#include <Arduino.h>
#include <MyDisplay.h>
#include <MyNetworkInterface.h>

// stating display
MyDisplay display;

// starting WiFi AP connection
const char* ssid_AP = "canReader";
const char* password_AP = "123stella";
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 100);  
IPAddress subnet(255, 255, 255, 0);
MyNetworkInterface NET(ssid_AP, password_AP, local_IP, gateway, subnet, &display);

void setup() {
    Serial.begin(115200);
   
   //display.begin();
   // display.writeScreenHello("Hello, world!", 500);

    if(NET.WifiInit())
      NET.displayInfo();
    else
      Serial.println("WiFi initialization failed");
    NET.UDPbegin(1234);
}

void loop() {
  NET.processReceivedData();
    //Serial.println("Hello, world!");
}
*/
#include <Arduino.h>
#include <MyDisplay.h>
#include <MyNetTCP.h>

// starting display
MyDisplay display;

// starting WiFi AP connection
const char* ssid_AP = "canReader";
const char* password_AP = "123stella";
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 100);  
IPAddress subnet(255, 255, 255, 0);
MyNetTCP NET(ssid_AP, password_AP, local_IP, gateway, subnet, &display);

// Data received callback
void onDataReceived(const char* data, IPAddress remoteIP, uint16_t remotePort) {
    Serial.println("Data received:");
    Serial.println(data);
    Serial.print("From IP: ");
    Serial.print(remoteIP);
    Serial.print(", Port: ");
    Serial.println(remotePort);
}

void setup() {
    Serial.begin(115200);

    // Initialize display
    //display.begin();
    // display.writeScreenHello("Hello, world!", 500);

    // Initialize WiFi and network interface
    if (NET.WifiInit()) {
        NET.displayInfo();
    } else {
        Serial.println("WiFi initialization failed");
        return;
    }

    // Initialize the TCP server
    if (NET.TCPbegin(1234)) {
        Serial.println("TCP Server started on port 1234");
    } else {
        Serial.println("Failed to start TCP server");
        return;
    }

    //NET.onDataReceived(onDataReceived);
}

void loop() {
    //NET.processReceivedData();
    //delay(10); // Small delay to prevent overloading the CPU
}
