import socket
import threading
import time
import sys

class Client:
    def __init__(self, ip, port):
        self.ip = ip
        self.port = port
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.client_socket.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1024*1024)
        self.client_socket.bind(("0.0.0.0", self.port))
        self.client_socket_rec = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.client_socket_rec.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 1024*1024)
        self.client_socket_rec.bind(("0.0.0.0", 1235))
        self.listen = False  # Controlla se l'ascolto è attivo
        self.listen_thread = None  # Inizializza il thread come None
        self.pingData = []
        self.lock = threading.Lock()  # Lock per proteggere l'accesso ai dati condivisi

    def startListen(self, start=True):
        if start and not self.listen:  # Avvia solo se non è già in ascolto
            self.listen = True
            self.listen_thread = threading.Thread(target=self.listen_for_messages)
            self.listen_thread.daemon = True
            self.listen_thread.start()
            print("Inizio ascolto...")

        elif not start:
            self.listen = False  # Impedisce al thread di ricevere più messaggi
            if self.listen_thread is not None:
                self.listen_thread.join()  # Unisciti al thread per garantirne la terminazione
            print("Ascolto fermato.")
        else:
            print("Ascolto già attivo." if start else "Ascolto già fermato.")

    def listen_for_messages(self):
        print(f"Server in ascolto su {self.ip}:{self.port}")
        while self.listen:
            try:
                message, address = self.client_socket.recvfrom(1024)  # La dimensione del buffer
                print(f"Ricevuto messaggio: {message.decode()} da {address}")
            except Exception as e:
                print(f"Errore nella ricezione del messaggio: {e}")

    def send_message(self, message, ipTarget, portTarget):
        self.client_socket.sendto(message.encode(), (ipTarget, portTarget))

    def fastListen(self):
        while self.listen:
            try:
                message, address = self.client_socket_rec.recvfrom(160)  # La dimensione del buffer
                # Uso del lock per proteggere l'accesso a PingData
                with self.lock:
                    #print(len(message));
                    self.pingData.append(int.from_bytes(message[36:39], byteorder='big'))
            except Exception as e:
                print(f"Errore nella ricezione del messaggio: {e}")

    def pingTest(self):
        risp = input("Quanti thread vuoi creare? ")
        self.pingData = []
        print(f"Server in ascolto su {self.ip}:{self.port}")
        self.listen = True
        th = []
        for i in range(int(risp)):
            th.append(threading.Thread(target=self.fastListen))
            th[i].daemon = True
        print("Thread creati, si parte!")
        startTime = time.time()
        for i in th:
            i.start()

        while (time.time() - startTime) < 2:
            pass

        self.listen = False
        lostPk = 0
        with self.lock:  # Accedi ai dati protetti dal lock
            print(f"Ho ricevuto {len(self.pingData)} pacchetti")
            if self.pingData:
                print(f"Il numero più grande che ho trovato è {max(self.pingData)}")

        # Stima pacchetti persi (opzionale)
        # per un altro approccio, potresti voler tenere traccia dei pacchetti inviati per confrontarli

        print(f"Ho perso {lostPk} pacchetti")

    def close(self):
        self.client_socket.close()
        print("Socket chiuso.")


def handle_user_input(client_instance):
    while True:
        print('Seleziona l\'operazione:\n')
        print('0. Chiudi socket\n')
        print('1. Invia Connection Request\n')
        print('2. PingTest\n')
        print('3. StartEcho\n')
        print('4. StopEcho\n')
        print('5. Uscita\n')
        risp = input('Operazione: ')

        if risp == '0':
            print("Chiudendo il socket...")
            client_instance.close()
            break
        elif risp == '1':
            client_instance.send_message("ConnectionRequest", SERVER_IP, PORT)
            print("Messaggio inviato.")
        elif risp == '2':
            client_instance.send_message("PingTest", SERVER_IP, PORT)
            client_instance.pingTest()
            print("Messaggio inviato.")
        elif risp == '3':
            client_instance.startListen(True)
        elif risp == '4':
            client_instance.startListen(False)
        elif risp == '5':
            print("Uscita dal programma...")
            client_instance.close()
            break
        else:
            print("Opzione non valida, riprova.")


# Definisci l'IP e la porta del server
SERVER_IP = "192.168.1.100"
PORT = 1234

# Crea un oggetto Client
client_instance = Client("192.168.1.100", PORT)

# Gestisci le operazioni dell'utente
handle_user_input(client_instance)
