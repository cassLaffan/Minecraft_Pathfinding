# Server: https://docs.python.org/3/library/socketserver.html#asynchronous-mixins
# Sending data: https://docs.python.org/3/library/struct.html#functions-and-exceptions

import socketserver
from struct import pack, unpack
import threading

slam = None

class RPCHandler(socketserver.BaseRequestHandler):
    def handle(self):
        while True:
            fn, = unpack('!b', self.request.recv(1))
            print(f"function: {fn}")
            if fn == 1:
                self.get_location()
            elif fn == 2:
                self.get_nodes()
            elif fn == 3:
                pass # upsert_nodes, unimplemented because no graphics 
            elif fn == 10:
                self.start_recording()
            elif fn == 11:
                self.stop_recording()
            elif fn == 12:
                self.reset_recording()
    
    def get_location(self):
        x, y, z, yaw = slam.get_location_from_gps()
        self.request.sendall(pack('!ffff', x, y, z, yaw))
    
    def get_nodes(self):
        nodes = slam.get_nodes()

        self.request.send(pack('!i', len(nodes)))

        for node in nodes:
            nodeId, playerId, seqId, x, y, z = node
            self.request.sendall(pack('!iiifff', nodeId, playerId, seqId, x, y, z))

    def start_recording(self):
        slam.start_recording()

    def stop_recording(self):
        slam.start_recording()

    def reset_recording(self):
        slam.reset_recording()

class ThreadedRPCHandler(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

def start_rpc_server(_slam, host, port):
    global slam
    slam = _slam
    server = ThreadedRPCHandler((host, port), RPCHandler)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.daemon = True
    server_thread.start()
