from django.core.management.base import BaseCommand, CommandError
import socket   
import threading
import environ


env = environ.Env()
environ.Env.read_env()

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = str(env("SERVER_SOCKET"))
port = int(env("PORT_SOCKET"))



server.bind((host, port ))
server.listen()
print(f"Server running on {host}:{port}")

class Command(BaseCommand):

    def __init__(self) -> None:
        self.clients = []

    def handle(self, *args, **options):
        while True:
            client, address = server.accept()
            username = client.recv(1024).decode('utf-8')

            #Al agregar un cliente establecer si es device o google ext
            # Buscarlo en el listado y si existe se reemplaza {client}
            self.clients.append(client)

            print(type(client))

            print(f"{username} is connected with {str(address)}")
            thread = threading.Thread(target=self.handle_messages, args=(client,))
            thread.start()



    def handle_messages(self, client):
        while True:
            try:
                message = client.recv(1024)
                self.broadcast(message, client)
            except:
                self.remove_client(client)
                client.close()
                break


    def broadcast(self,message, _client):
        for client in self.clients:
            if client != _client:
                try:
                    client.send(message)
                except:
                    #remove_client(client)
                    print(f"Cannot send message to {client}")
                    pass
    
    def remove_client(self,client):
        index = self.clients.index(client)
        #username = usernames[index]
        #clients.remove(client)
        #usernames.remove(username)