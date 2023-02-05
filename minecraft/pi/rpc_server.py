# Server: https://docs.python.org/3/library/socketserver.html#asynchronous-mixins
# Sending data: https://docs.python.org/3/library/struct.html#functions-and-exceptions

import socketserver
from struct import pack, unpack
import threading

class RPCHandler(socketserver.BaseRequestHandler):
    slam = None

    def __init__(self, slam):
        self.slam = slam

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
                self.start_recording()
            elif fn == 5:
                self.stop_recording()
            elif fn == 6:
                self.reset_recording()
    
    def get_location(self):
        x, y, z, yaw = self.slam.get_location_from_gps()
        self.request.sendall(pack('!ffff', x, y, z, yaw))
    
    def get_nodes(self):
        nodes = self.slam.get_nodes()

        self.request.send(pack('!i', len(nodes)))

        for node in nodes:
            nodeId, playerId, seqId, x, y, z = node
            self.request.sendall(pack('!iiifff', nodeId, playerId, seqId, x, y, z))

    def start_recording(self):
        self.slam.start_recording()

    def stop_recording(self):
        self.slam.start_recording()

    def reset_recording(self):
        self.slam.reset_recording()

class ThreadedRPCHandler(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

def start_rpc_server(slam, host, port):
    server = ThreadedRPCHandler((host, port), RPCHandler(slam))
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.daemon = True
    server_thread.start()
