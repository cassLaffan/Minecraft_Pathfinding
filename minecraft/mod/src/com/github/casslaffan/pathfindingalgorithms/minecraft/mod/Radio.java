package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import net.minecraft.client.MinecraftClient;
import net.minecraft.network.packet.s2c.play.GameMessageS2CPacket;
import org.spongepowered.asm.mixin.injection.callback.CallbackInfo;

import java.util.ArrayDeque;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class Radio {
    Mod mod = Mod.getInstance();
    MinecraftClient mc = mod.mc;
    Graph graph = mod.graph;

    ScheduledExecutorService radioQueueExecutor = Executors.newSingleThreadScheduledExecutor();

    double range = 20;

    private static final String RADIO_IDENTIFIER = "radio:";

    /*
     RADIO MESSAGE FORMAT
     h:e:a:d:e:r:s:d|a|t|a
     radio:x:y:z:type:|data
     radio:56.4:84.8:34.1:node:1|56.4|84.8|34.1
    */

    public Radio() {
        radioQueueExecutor.scheduleAtFixedRate(this::sendRadioMessage, 0, 1200, TimeUnit.MILLISECONDS);
    }

    public void onGameMessage(GameMessageS2CPacket packet, CallbackInfo ci) {
        if (mc.player == null) return;

        String chatMessage = packet.getMessage().getString();

        // Chat messages come in the format `<Player> Hello, World!` so you need to find the beginning to the actual message
        int radioIndex = chatMessage.indexOf(RADIO_IDENTIFIER);
        if (radioIndex < 0) {
            return;
        }

        ci.cancel();

        if (packet.getSenderUuid().equals(mc.player.getUuid())) {
            return;
        }

        String[] radioMessages = chatMessage.substring(radioIndex).split("\\+");

        for (String radioMessage : radioMessages) {
            String[] radioHeaders = radioMessage.split(":");

            float x = Float.parseFloat(radioHeaders[1]);
            float y = Float.parseFloat(radioHeaders[2]);
            float z = Float.parseFloat(radioHeaders[3]);
            String radioType = radioHeaders[4];
            String data = radioHeaders[5];

            if (distance(x, y, z, mc.player.getX(), mc.player.getY(), mc.player.getZ()) > range) {
                return;
            }

            switch (radioType) {
                case "node":
                    receiveNode(data, x, y, z);
                    break;
            }
        }
    }

    void
    sendNode(Node node) {
        String data = String.format("%d", node.id);
        queueRadioMessage("node", data);
    }

    void receiveNode(String rawData, float x, float y, float z) {
        int id = Integer.parseInt(rawData);

        Node node = new Node(id, x, y, z);

        graph.addNode(node);
    }

    ArrayDeque<String> queue = new ArrayDeque<>();
    void queueRadioMessage(String type, String data) {
        assert mc.player != null;
        queue.add(makeHeaders(type) + data);
    }

    void sendRadioMessage() {
        if (queue.isEmpty()) {
            return;
        }

        StringBuilder radioMessages = new StringBuilder(256);
        while (!queue.isEmpty() && queue.peekFirst().length() + radioMessages.length() < 256) {
            radioMessages.append(queue.remove());
            radioMessages.append("+");
        }
        radioMessages.setLength(Math.max(radioMessages.length() - 1, 0));

        if (mc.player != null) {
            mc.player.sendChatMessage(radioMessages.toString());
        }
    }

    private String makeHeaders(String type) {
        assert mc.player != null;
        return String.format("radio:%.1f:%.1f:%.1f:%s:", mc.player.getX(), mc.player.getY(), mc.player.getZ(), type);
    }

    private double distance(double ax, double ay, double az, double bx, double by, double bz) {
        return Math.sqrt(Math.pow(ax - bx, 2) + Math.pow(ay - by, 2) + Math.pow(az - bz, 2));
    }
}
