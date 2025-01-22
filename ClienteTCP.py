import socket
import time
from datetime import datetime
from pynput import keyboard
import threading

#IP y puerto del ESP8266
HOST = "192.168.1.36"
PORT = 80

teclaMuteo="ç"
muteado="       "

#Detecta si presionas la tecla de muteo
def tecla_en_segundo_plano():
    global muteado
    def on_press(key):
        global muteado
        try:
            if key.char == teclaMuteo:
                if muteado=="       ":
                    muteado="MUTEADO"
                else:
                    muteado="       "

        except AttributeError:
            pass

    listener = keyboard.Listener(on_press=on_press)
    listener.start()
    listener.join()

#Crea Thread
threading.Thread(target=tecla_en_segundo_plano).start()

#Conectarse y enviar mensaje por TCP
def enviar(mensaje):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        mensaje=mensaje+ "\n"
        s.connect((HOST, PORT))
        s.sendall(mensaje.encode())
        print(mensaje)
        time.sleep(0.3)  
        s.shutdown(socket.SHUT_WR)

#Obtiene la hora
def obtener_hora():
    ahora = datetime.now()
    return ahora.strftime("%H:%M:%S")

while True:
    try:
        # Uso de la función
        enviar(obtener_hora()+muteado)
    except:
        pass