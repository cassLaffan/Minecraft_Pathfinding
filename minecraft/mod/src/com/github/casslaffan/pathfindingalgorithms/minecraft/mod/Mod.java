package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import net.fabricmc.api.EnvType;
import net.fabricmc.api.Environment;
import net.fabricmc.api.ModInitializer;
import net.fabricmc.fabric.api.client.command.v1.ClientCommandManager;
import net.fabricmc.fabric.api.client.rendering.v1.WorldRenderEvents;
import net.minecraft.client.MinecraftClient;
import net.minecraft.text.LiteralText;
import net.minecraft.util.math.Vec3d;

import java.util.Random;
import java.util.concurrent.*;

@Environment(EnvType.CLIENT)
public class Mod extends Thread implements ModInitializer {
    public static Mod instance;

    public MinecraftClient mc;
    public Graph graph;
    public RpcServer rpc;
    public Radio radio;

    ClientCommands clientCommands;
    RpcCommands rpcCommands;

    Random r = new Random();
    ScheduledExecutorService executor = Executors.newSingleThreadScheduledExecutor();
    ScheduledFuture<?> recordHandler;

    Vec3d lastPos = new Vec3d(0.0, 0.0, 0.0);
    boolean recording = false;
    int intervalMs = 250;
    double jitterFactor = 0.8;

    @Override
    public void onInitialize() {
        instance = this;
        mc = MinecraftClient.getInstance();
        graph = new Graph();
        rpc = new RpcServer(25566);
        radio = new Radio();
        clientCommands = new ClientCommands();
        rpcCommands = new RpcCommands();

        WorldRenderEvents.BEFORE_DEBUG_RENDER.register(graph);
        clientCommands.registerCommands(ClientCommandManager.DISPATCHER);
        rpcCommands.registerCommands();

        rpc.start();
        recordHandler = executor.scheduleAtFixedRate(this::record, 0, intervalMs, TimeUnit.MILLISECONDS);
        executor.scheduleAtFixedRate(this::status, 0, 200, TimeUnit.MILLISECONDS);
    }

    void status() {
        if (mc.player == null) return;

        String statusText = String.format(
                "[%s]%s id:%d seq:%d interval:%dms jitter:%.2f range:%.1f help:/pf",
                recording ? "Recording" : "Paused",
                graph.hidden ? " (HIDDEN)" : "",
                Node.clientPlayerId,
                Node.clientSequenceId,
                intervalMs,
                jitterFactor,
                radio.range
        );

        mc.player.sendMessage(new LiteralText(statusText), true);
    }

    public void record() {
        if (mc.player == null) return;

        if (recording) {
            Vec3d currPos = mc.player.getPos();

            if (!currPos.isInRange(lastPos, 0.1)) {
                Node node = new Node(currPos.add(r.nextGaussian()* jitterFactor, Math.abs(r.nextGaussian()* jitterFactor), r.nextGaussian()* jitterFactor));
                graph.addNode(node);
                radio.sendNode(node);
            }

            lastPos = currPos;
        }
    }

    public static Mod getInstance() {
        return instance;
    }
}
