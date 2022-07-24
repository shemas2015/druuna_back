from multiprocessing.sharedctypes import Value
from django.core.management.base import BaseCommand, CommandError
import socket   
import threading
import environ


env = environ.Env()
environ.Env.read_env()






class ClientSocket:
    socket_device = None
    socket_web = None
    name = None
    mac = None

    def __init__(self):
        pass

    def get_socket_device(self):
        return self.socket_device

    def set_socket_device(self,socket_device):
        self.socket_device = socket_device

    def get_socket_web(self):
        return self.socket_web

    def set_socket_web(self,socket_web):
        self.socket_web = socket_web

    def get_mac(self):
        return self.mac

    def set_mac(self,mac):
        self.mac = mac





class Command(BaseCommand):

    def __init__(self) -> None:
        self.clients = []
        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def handle(self, *args, **options):
        host = str(env("SERVER_SOCKET"))
        port = int(env("PORT_SOCKET"))
        

        self.server.bind((host, port))
        self.server.listen()
        self.receive_connections()



    def send_message( self,receiver , message ):
        try:
            receiver.send(message)
        except:
            print("Can't send message")
        pass


            
    def handle_socket_web_msg(self,client: ClientSocket):
        while True:
            try:
                socket_web_msg = client.get_socket_web().recv(1024)
                if not socket_web_msg:
                    client.set_socket_web(None)
                    break
                
                if( client.get_socket_web() is not None ):
                    lst = range(0, int(socket_web_msg))
                    for i in range(0, len(lst), 9):
                        self.send_message( client.get_socket_device(), "{}".format(len(lst[i:i + 9])).encode("utf-8") )
            except ValueError as e:
                print("{} no es un valor numérico válido".format(socket_web_msg))
            except Exception as ex:
                client.get_socket_web().close()
                break
        

    def handle_socket_device_msg(self,client: ClientSocket):
        while True:
            try:
                socket_device_msg = client.get_socket_device().recv(1024)
                if not socket_device_msg:
                    client.set_socket_device(None)
                    break

                if( client.get_socket_device() is not None ):
                    self.send_message( client.get_socket_web(), socket_device_msg )
            except:
                client.get_socket_device().close()
                break

    def receive_connections(self):
        while True:
            try:
                client, address = self.server.accept()
                mac = client.recv(1024).decode('utf-8')
                type = client.recv(1024).decode('utf-8')

                #Busca la mac entre listado de clientes vinculados, si existe modifica socket_device o socket_web
                new_client = None
                for sk_client in self.clients:
                    if sk_client.get_mac() == mac:
                        new_client = sk_client

                if new_client is None:
                    new_client = ClientSocket()
                    new_client.set_mac(mac)
                    self.clients.append(new_client)


                if(type == '1'):
                    new_client.set_socket_device(client)
                    #Desde el integrado ESP32
                    thread = threading.Thread(target=self.handle_socket_device_msg, args=(new_client,))
                    thread.start()

                elif(type =='2'):
                    new_client.set_socket_web(client)
                    #Desde el navegador
                    thread = threading.Thread(target=self.handle_socket_web_msg, args=(new_client,))
                    thread.start()
                    
                print(f"{mac} is connected with {str(address)}")
            except Exception as ex:
                print(ex)
            



