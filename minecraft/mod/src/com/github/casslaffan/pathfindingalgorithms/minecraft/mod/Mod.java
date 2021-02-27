package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import com.mojang.brigadier.arguments.IntegerArgumentType;
import net.fabricmc.api.EnvType;
import net.fabricmc.api.Environment;
import net.fabricmc.api.ModInitializer;
import net.fabricmc.fabric.api.client.rendering.v1.WorldRenderEvents;
import net.fabricmc.fabric.api.command.v1.CommandRegistrationCallback;
import net.minecraft.client.MinecraftClient;
import net.minecraft.entity.effect.StatusEffect;
import net.minecraft.entity.effect.StatusEffectInstance;
import net.minecraft.entity.effect.StatusEffectType;
import net.minecraft.entity.effect.StatusEffects;
import net.minecraft.server.command.CommandManager;
import net.minecraft.text.LiteralText;
import net.minecraft.util.math.Vec3d;
import org.lwjgl.system.CallbackI;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Iterator;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

@Environment(EnvType.CLIENT)
public class Mod extends Thread implements ModInitializer {
    MinecraftClient mc = MinecraftClient.getInstance();
    Renderer renderer = new Renderer();
    RpcServer rpc = new RpcServer(25566);

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
        rpc.register(RPC_GET_LOCATION, this::getLocation);
        rpc.register(RPC_GET_NODES, this::getNodes);
        rpc.register(RPC_UPSERT_GRAPH, this::upsertGraph);
        rpc.register(RPC_RESET_GRAPH, this::resetGraph);
        rpc.start();

        WorldRenderEvents.BEFORE_DEBUG_RENDER.register(renderer);

        // :)
        CommandRegistrationCallback.EVENT.register((dispatcher, dedicated) -> {
            dispatcher.register(CommandManager.literal("start").executes(ctx -> { recording = true; return 1; }));
            dispatcher.register(CommandManager.literal("stop").executes(ctx -> { recording = false; return 1; }));
            dispatcher.register(CommandManager.literal("delete").executes(ctx -> { renderer.delete(); return 1; }));
            dispatcher.register(CommandManager.literal("reset").executes(ctx -> { renderer.reset(); return 1; }));
            dispatcher.register(CommandManager.literal("hide").executes(ctx -> { renderer.toggleHidden(); return 1; }));
            dispatcher.register(CommandManager.literal("blind").executes(ctx -> {
                if (mc.player == null) return 0;

                if (mc.player.hasStatusEffect(StatusEffects.BLINDNESS)) {
                    mc.player.removeStatusEffect(StatusEffects.BLINDNESS);
                } else {
                    mc.player.addStatusEffect(
                            new StatusEffectInstance(StatusEffects.BLINDNESS, 300 * 20, 0, false, false)
                    );
                }

                return 1;
            }));
            dispatcher.register(CommandManager.literal("interval")
                    .then(CommandManager.argument("ms", IntegerArgumentType.integer(100))
                            .executes(ctx -> {
                                int ms = IntegerArgumentType.getInteger(ctx, "ms");
                                recordHandler.cancel(true);
                                recordHandler = recordService.scheduleAtFixedRate(this::record, 0, ms, TimeUnit.MILLISECONDS);
                                return 1;
                            })));
        });

        recordHandler = recordService.scheduleAtFixedRate(this::record, 0, 500, TimeUnit.MILLISECONDS);
    }

    public void record() {
        if (mc.player == null) return;

        if (recording) {
            Vec3d currPos = mc.player.getPos();

            if (!currPos.isInRange(lastPos, 0.1)) {
                renderer.addNode(currPos);
            }

            lastPos = currPos;
        }

        String statusText = String.format(
                "%s [%d nodes] (/start, /stop, /reset, /delete, /interval <ms>, /hide, /blind)",
                recording ? "Recording" : "Paused",
                renderer.numNodes()
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
            out.writeFloat(Util.yawToAbsoluteToRad(mc.player.yaw));
        }
        out.flush();
    }

    public void getNodes(DataInputStream in, DataOutputStream out) throws IOException {
        int len = renderer.numNodes();
        out.writeInt(len);

        Iterator<Node> iter = renderer.nodeMap.values().stream().sorted().iterator();
        Node node;
        while (iter.hasNext()) {
            node = iter.next();
            out.writeInt(node.id);
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

                renderer.upsertNode(nodeId, x, y, z, color, size);
            } else if (type == RPC_GRAPH_EDGE) {
                aId = in.readInt();
                bId = in.readInt();
                color = in.readInt();

                renderer.upsertEdge(aId, bId, color);
            } else {
                break;
            }
        }
    }

    public void resetGraph(DataInputStream in, DataOutputStream out) throws IOException {
        renderer.reset();
    }

    public void getObstructions(DataInputStream in, DataOutputStream out) throws IOException {
        // TODO: Ray cast between nodes to get obstructions https://fabricmc.net/wiki/tutorial:pixel_raycast
    }
}
