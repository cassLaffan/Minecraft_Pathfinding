package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import net.minecraft.client.MinecraftClient;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Iterator;

public class RpcCommands {
    Mod mod = Mod.getInstance();
    RpcServer rpc = mod.rpc;
    MinecraftClient mc = mod.mc;
    Graph graph = mod.graph;

    public static final byte CMD_GET_LOCATION = 1;
    public static final byte CMD_GET_NODES = 2;
    public static final byte CMD_UPSERT_GRAPH = 3;
    public static final byte CMD_RESET_GRAPH = 4;

    void registerCommands() {
        rpc.registerHandler(CMD_GET_LOCATION, this::getLocation);
        rpc.registerHandler(CMD_GET_NODES, this::getNodes);
        rpc.registerHandler(CMD_UPSERT_GRAPH, this::upsertGraph);
        rpc.registerHandler(CMD_RESET_GRAPH, this::resetGraph);
    }

    private void getLocation(DataInputStream in, DataOutputStream out) throws IOException {
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

    private void getNodes(DataInputStream in, DataOutputStream out) throws IOException {
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

    private void upsertGraph(DataInputStream in, DataOutputStream out) throws IOException {
        byte RPC_GRAPH_NODE = 1;
        byte RPC_GRAPH_EDGE = 2;

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

    private void resetGraph(DataInputStream in, DataOutputStream out) throws IOException {
        graph.reset();
    }

    private void getObstructions(DataInputStream in, DataOutputStream out) throws IOException {
        // TODO: Ray cast between nodes to get obstructions https://fabricmc.net/wiki/tutorial:pixel_raycast
    }

}
