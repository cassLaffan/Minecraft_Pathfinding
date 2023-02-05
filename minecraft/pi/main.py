from rpc_server import start_rpc_server
from slam import Slam

def main():
    slam = Slam()
    print("started rpc server")
    start_rpc_server(slam, "0.0.0.0", 25565)
    print("started slam")
    slam.start_slaming()

if __name__  == "__main__":
    main()
