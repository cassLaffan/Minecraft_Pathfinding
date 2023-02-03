# Server: https://docs.python.org/3/library/socketserver.html#asynchronous-mixins
# Sending data: https://docs.python.org/3/library/struct.html#functions-and-exceptions

import socketserver
from struct import pack, unpack
import threading
import socket

class MyTCPHandler(socketserver.BaseRequestHandler):
    def handle(self):
        while True:
            fn = unpack('!b', self.request.recv(1))
            if fn == 1:
                self.get_location()
            elif fn == 2:
                self.get_nodes()
            elif fn == 3:
                pass # upsert_nodes, unimplemented because no graphics 
            elif fn == 4:
                self.start()
            elif fn == 5:
                self.stop()
            elif fn == 6:
                self.reset()
    
    def get_location(self):
        x, y, z, yaw = 1.0, 2.0, 3.0, 4.0
        self.request.sendall(pack('!ffff', x, y, z, yaw))
    
    def get_nodes(self):
        nodes = [(1, 1, 1, 3, 4, 5), (2, 1, 2, 3, 4, 5)]

        self.request.send(pack('!i', len(nodes)))

        for node in nodes:
            nodeId, playerId, seqId, x, y, z = node
            self.request.sendall(pack('!iiifff', nodeId, playerId, seqId, x, y, z))

    def start_recording(self):
        pass

    def stop_recording(self):
        pass

    def reset_recording(self):
        pass

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

def start_rpc_server(host, port):
    server = ThreadedTCPServer((host, port), MyTCPHandler)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.daemon = True
    server_thread.start()
