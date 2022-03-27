package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import net.fabricmc.api.EnvType;
import net.fabricmc.api.Environment;
import net.fabricmc.api.ModInitializer;
import net.fabricmc.fabric.api.client.rendering.v1.WorldRenderEvents;
import net.minecraft.client.MinecraftClient;
import net.minecraft.entity.effect.StatusEffectInstance;
import net.minecraft.entity.effect.StatusEffects;
import net.minecraft.text.LiteralText;
import net.minecraft.util.math.Vec3d;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Random;
import java.util.concurrent.*;
import java.util.function.Consumer;

@Environment(EnvType.CLIENT)
public class Mod extends Thread implements ModInitializer {
    public static Mod instance;

    public MinecraftClient mc = MinecraftClient.getInstance();
    public Graph graph;
    public RpcServer rpc;
    public Radio radio;
    public HashMap<String, Consumer<String[]>> commands = new HashMap<>();

    boolean recording = false;
    ScheduledExecutorService recordService = Executors.newSingleThreadScheduledExecutor();
    ScheduledFuture<?> recordHandler;
    Vec3d lastPos = new Vec3d(0.0, 0.0, 0.0);

    public static final byte RPC_GET_LOCATION = 1;
    public static final byte RPC_GET_NODES = 2;
    public static final byte RPC_UPSERT_GRAPH = 3;
    public static final byte RPC_RESET_GRAPH = 4;

    public static final byte RPC_GRAPH_NODE = 1;
    public static final byte RPC_GRAPH_EDGE = 2;

    @Override
    public void onInitialize() {
        instance = this;

        graph = new Graph();
        rpc = new RpcServer(25566);
        radio = new Radio();

        rpc.register(RPC_GET_LOCATION, this::getLocation);
        rpc.register(RPC_GET_NODES, this::getNodes);
        rpc.register(RPC_UPSERT_GRAPH, this::upsertGraph);
        rpc.register(RPC_RESET_GRAPH, this::resetGraph);
        rpc.start();

        WorldRenderEvents.BEFORE_DEBUG_RENDER.register(graph);

        commands.put("/start", args -> recording = true);
        commands.put("/stop", args -> recording = false);
        commands.put("/delete", args -> graph.delete());
        commands.put("/reset", args -> graph.reset());
        commands.put("/hide", args -> graph.toggleHidden());
        commands.put("/blind", args -> {
            if (mc.player == null) return;

            if (mc.player.hasStatusEffect(StatusEffects.BLINDNESS)) {
                mc.player.removeStatusEffect(StatusEffects.BLINDNESS);
            } else {
                mc.player.addStatusEffect(
                        new StatusEffectInstance(StatusEffects.BLINDNESS, 300 * 20, 0, false, false)
                );
            }
        });
        commands.put("/interval", args -> {
            int ms = Integer.parseInt(args[1]);
            recordHandler.cancel(true);
            recordHandler = recordService.scheduleAtFixedRate(this::record, 0, ms, TimeUnit.MILLISECONDS);
        });
        commands.put("/range", args -> {
            double blocks = Double.parseDouble(args[1]);
            radio.range = blocks;
        });
        commands.put("/id", args -> {
            if (mc.player == null) return;

            if (args.length == 1) {
                mc.player.sendMessage(new LiteralText("" + Node.clientPlayerId), false);
            } else {
                Node.clientPlayerId = Integer.parseInt(args[1]);
                Node.clientSequenceId = 0;
            }
        });

        recordHandler = recordService.scheduleAtFixedRate(this::record, 0, 500, TimeUnit.MILLISECONDS);
    }

    public boolean handleCommand(String command) {
        try {
            String[] args = command.split(" ");
            if (commands.containsKey(args[0])) {
                commands.get(args[0]).accept(args);
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return false;
    }

    public void record() {
        if (mc.player == null) return;

        if (recording) {
            Vec3d currPos = mc.player.getPos();

            if (!currPos.isInRange(lastPos, 0.1)) {
                Node node = new Node(currPos);
                graph.addNode(node);
                radio.sendNode(node);
            }

            lastPos = currPos;
        }

        String statusText = String.format(
                "%s [%d nodes] (/start, /stop, /reset, /delete, /hide, /blind, /interval <ms>, /range <blocks>, /id [id])",
                recording ? "Recording" : "Paused",
                graph.numNodes()
        );

        mc.player.sendMessage(new LiteralText(statusText), true);
    }

    public void getLocation(DataInputStream in, DataOutputStream out) throws IOException {
        if (mc.player == null) {
            out.writeFloat(0.0F);
            out.writeFloat(0.0F);
            out.writeFloat(0.0F);
            out.writeFloat(0.0F);
        } else {
            out.writeFloat((float) mc.player.getX());
            out.writeFloat((float) mc.player.getY());
            out.writeFloat((float) mc.player.getZ());
            out.writeFloat(Util.yawToAbsoluteToRad(mc.player.headYaw));
        }
        out.flush();
    }

    public void getNodes(DataInputStream in, DataOutputStream out) throws IOException {
        int len = graph.numNodes();
        out.writeInt(len);

        Iterator<Node> iter = graph.nodeMap.values().stream().sorted().iterator();
        Node node;
        while (iter.hasNext()) {
            node = iter.next();
            out.writeInt(node.id);
            out.writeInt(node.playerId);
            out.writeInt(node.sequenceId);
            out.writeFloat(node.x);
            out.writeFloat(node.y);
            out.writeFloat(node.z);
        }

        out.flush();
    }

    public void upsertGraph(DataInputStream in, DataOutputStream out) throws IOException {
        byte type;
        int nodeId, aId, bId, color;
        float x, y, z;
        float size;

        while (true) {
            type = in.readByte();
            if (type == RPC_GRAPH_NODE) {
                nodeId = in.readInt();
                x = in.readFloat();
                y = in.readFloat();
                z = in.readFloat();
                color = in.readInt();
                size = in.readFloat();

                graph.upsertNode(nodeId, x, y, z, color, size);
            } else if (type == RPC_GRAPH_EDGE) {
                aId = in.readInt();
                bId = in.readInt();
                color = in.readInt();

                graph.upsertEdge(aId, bId, color);
            } else {
                break;
            }
        }
    }

    public void resetGraph(DataInputStream in, DataOutputStream out) throws IOException {
        graph.reset();
    }

    public void getObstructions(DataInputStream in, DataOutputStream out) throws IOException {
        // TODO: Ray cast between nodes to get obstructions https://fabricmc.net/wiki/tutorial:pixel_raycast
    }

    public static Mod getInstance() {
        return instance;
    }
}
