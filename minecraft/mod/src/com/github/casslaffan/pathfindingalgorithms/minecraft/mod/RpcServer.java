package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;

public class RpcServer extends Thread {
    int port;
    HashMap<Byte, Handler> handlers = new HashMap<>();

    public RpcServer(int port) {
        this.port = port;
    }

    @Override
    public void run() {
        try (ServerSocket socket = new ServerSocket(port)) {
            while (!isInterrupted()) {
                new ClientHandler(socket.accept()).start();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void register(byte type, Handler handler) {
        handlers.put(type, handler);
    }

    @FunctionalInterface
    public interface Handler {
        void apply(DataInputStream in, DataOutputStream out) throws IOException;
    }

    private class ClientHandler extends Thread {
        Socket socket;

        private ClientHandler(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            try (
                DataInputStream in = new DataInputStream(new BufferedInputStream(socket.getInputStream()));
                DataOutputStream out = new DataOutputStream(new BufferedOutputStream(socket.getOutputStream()));
            ) {
                byte type;

                while (!isInterrupted()) {
                    type = in.readByte();
                    handlers.getOrDefault(type, (a, b) -> {}).apply(in, out);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
