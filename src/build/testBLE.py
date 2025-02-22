import socket

def connect_to_server():
    # Creazione del socket TCP
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # IP del server (puoi sostituirlo con l'IP effettivo del server a cui vuoi connetterti)
    server_ip = '192.168.1.100'  # In questo esempio si connette al server ESP32
    server_port = 1234  # Porta del server

    try:
        # Connessione al server
        client_socket.connect((server_ip, server_port))
        print(f"Connesso al server {server_ip} sulla porta {server_port}")

        # Invia un messaggio al server
        message = "ConnectionRequest"
        client_socket.sendall(message.encode())  # Invia il messaggio al server
        print(f"Messaggio inviato: {message}")
        
        # Ricevi la risposta dal server
        response = client_socket.recv(1024)  # Dimensione del buffer di ricezione
        print(f"Risposta dal server: {response.decode()}")

    except Exception as e:
        print(f"Errore nella connessione: {e}")

    finally:
        # Chiudi il socket
        client_socket.close()
        print("Connessione chiusa.")

if __name__ == "__main__":
    connect_to_server()