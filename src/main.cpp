#include <WiFi.h>
#include "MyNetTCP.h"   // Includi la classe che hai fornito
#include "MyDisplay.h"  // Includi la classe per il display (se utilizzato)

MyDisplay display;  // Crea un oggetto per il display (se utilizzato)

// Parametri di configurazione Wi-Fi (Access Point)
const char* ssid = "canReader";
const char* password = "123stella";

// Configura l'IP dell'ESP32 (Access Point)
IPAddress local_IP(192, 168, 1, 1);  // Indirizzo IP statico
IPAddress gateway(192, 168, 1, 1);   // Gateway (tipicamente uguale a local_IP)
IPAddress subnet(255, 255, 255, 0);  // Subnet mask

// Crea un oggetto della classe MyNetTCP
MyNetTCP netTCP(ssid, password, local_IP, gateway, subnet, &display);

void setup() {
    Serial.begin(115200);
    
    // Inizializza la connessione Wi-Fi in modalità Access Point
    if (!netTCP.WifiInit()) {
      Serial.println("Impossibile avviare l'Access Point");
      return;
    }
  
    // Mostra l'IP dell'ESP32 sul display (se presente)
    netTCP.displayInfo();
  
    // Inizializza il server TCP sulla porta 1234
    if (!netTCP.TCPbegin(1234)) {
      Serial.println("Impossibile avviare il server TCP");
      return;
    }
  }
  
  void loop() {
    // Controlla e gestisci i dati ricevuti dai client
    netTCP.processReceivedData();
    
}
