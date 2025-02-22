import socket
import time

# Parametri del server ESP32
ESP32_IP = '192.168.1.1'  # Indirizzo IP dell'ESP32 (sostituisci con quello effettivo)
ESP32_PORT = 1234         # Porta sulla quale il server TCP è in ascolto

def connect_to_server():
    # Crea un oggetto socket per la connessione TCP
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Connetti al server ESP32
        print(f"Connettendo al server {ESP32_IP}:{ESP32_PORT}...")
        client_socket.connect((ESP32_IP, ESP32_PORT))
        print("Connessione stabilita!")
        
        # Invia un messaggio al server
        message = "ConnectionRequest"
        print(f"Inviando messaggio: {message}")
        client_socket.sendall(message.encode())

        # Attendi una risposta dal server
        response = client_socket.recv(1024).decode()
        print(f"Risposta dal server: {response}")

        # Invia un altro messaggio (ad esempio, PingTest)
        message = "PingTest"
        print(f"Inviando messaggio: {message}")
        client_socket.sendall(message.encode())
        
        # Attendi una risposta dal server
        response = client_socket.recv(1024).decode()
        print(f"Risposta dal server: {response}")

    except Exception as e:
        print(f"Errore nella connessione o nella comunicazione: {e}")
    
    finally:
        # Chiudi la connessione
        print("Chiusura della connessione...")
        client_socket.close()

if __name__ == '__main__':
    # Esegui la funzione di connessione
    connect_to_server()
