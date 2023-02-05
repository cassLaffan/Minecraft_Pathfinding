import time

class Slam():
    nodes = []
    is_recording = False

    last_x = 0.0

    player_id = 1
    node_id = 1 # Node id is a global ID unique across all players
    seq_id = 1 # sequence ID is the id of the node unique to 

    def start_slaming(self):
        try:
            while True:
                if self.is_recording:
                    x, y, z, yaw = self.get_location_from_gps()
                    self.nodes.append((self.node_id, self.player_id, self.seq_id, x, y, z, yaw))
                    self.node_id += 1
                    self.seq_id += 1
                time.sleep(1)
        except KeyboardInterrupt:
            print('interrupted!')   

    def get_location_from_gps(self):
        x, y, z, yaw = self.last_x, 0.0, 0.0, 0.0
        self.last_x += 1.0
        return (x, y, z, yaw)

    def get_nodes(self):
        return self.nodes

    def start_recording(self):
        self.is_recording = True

    def stop_recording(self):
        self.is_recording = False

    def reset_recording(self):
        self.nodes = []
