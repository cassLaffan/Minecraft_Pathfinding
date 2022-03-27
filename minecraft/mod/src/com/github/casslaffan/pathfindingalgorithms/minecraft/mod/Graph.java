package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import com.mojang.blaze3d.systems.RenderSystem;
import it.unimi.dsi.fastutil.ints.Int2ObjectOpenHashMap;
import it.unimi.dsi.fastutil.longs.Long2ObjectOpenHashMap;
import net.fabricmc.fabric.api.client.rendering.v1.WorldRenderContext;
import net.fabricmc.fabric.api.client.rendering.v1.WorldRenderEvents;
import net.fabricmc.loader.api.FabricLoader;
import net.minecraft.client.MinecraftClient;
import net.minecraft.client.render.BufferBuilder;
import net.minecraft.client.render.Tessellator;
import net.minecraft.client.render.VertexFormat;
import net.minecraft.client.render.VertexFormats;
import net.minecraft.client.render.debug.DebugRenderer;
import net.minecraft.util.math.Vec3d;
import org.lwjgl.opengl.GL11;

public class Graph implements WorldRenderEvents.DebugRender {
    boolean hidden = false;
    Int2ObjectOpenHashMap<Node> nodeMap = new Int2ObjectOpenHashMap<>(1_000);
    Long2ObjectOpenHashMap<Edge> edgeMap = new Long2ObjectOpenHashMap<>(10_000);

    @Override
    public void beforeDebugRender(WorldRenderContext context) {
        if (hidden) return;

        Vec3d pos = context.camera().getPos();

        Tessellator tessellator = Tessellator.getInstance();
        BufferBuilder bufferBuilder = tessellator.getBuffer();

        // This sets up a bunch of global renderer variables that I don't know how to do myself.
        DebugRenderer.drawString("x", 0, 0, 0, Util.DEFAULT_COLOR);

        for(Node node : nodeMap.values()) {
            DebugRenderer.drawString(node.sequenceId + "", node.x, node.y + 0.5, node.z, node.color, node.size);
        }

        RenderSystem.lineWidth(1.25F);
        bufferBuilder.begin(VertexFormat.DrawMode.LINES, VertexFormats.POSITION_COLOR);
        for (Edge edge : edgeMap.values()) {
            bufferBuilder.vertex(edge.a.x - pos.x, edge.a.y - pos.y + .25, edge.a.z - pos.z)
                    .color(edge.cr, edge.cg, edge.cb, edge.ca)
                    .next();

            bufferBuilder.vertex(edge.b.x - pos.x, edge.b.y - pos.y + .25, edge.b.z - pos.z)
                    .color(edge.cr, edge.cg, edge.cb, edge.ca)
                    .next();
        }

        tessellator.draw();
    }

    void addNode(Node node) {
        nodeMap.put(node.id, node);
    }

    void upsertNode(int id, float x, float y, float z, int color, float size) {
        if (id < 0) return;

        if ((color & 0xFF) == 0) {
            nodeMap.remove(id);
        } else {
            nodeMap.computeIfAbsent(id, k -> new Node(k, x, y, z)).update(x, y, z, color, size);
        }
    }

    void upsertEdge(int aId, int bId, int color) {
        // Deduplicate a != b AND a,b == b,a
        if (aId == bId) return;
        if (bId < aId) {
            int temp = aId;
            aId = bId;
            bId = temp;
        }

        long key = Util.packInts(aId, bId);

        if ((color & 0xFF) == 0) {
            edgeMap.remove(key);
        } else {
            Edge edge = edgeMap.get(key);
            if (edge == null) {
                Node a = nodeMap.get(aId);
                Node b = nodeMap.get(bId);
                if (a != null && b != null) {
                    edgeMap.put(key, new Edge(a, b, color));
                }
            } else {
                edge.update(color);
            }
        }
    }

    void delete() {
        nodeMap.clear();
        edgeMap.clear();
        Node.clientSequenceId = 0;
    }

    void reset() {
        edgeMap.clear();

        for (Node node : nodeMap.values()) {
            node.update(0, 0,0, Util.DEFAULT_COLOR, Util.DEFAULT_NODE_SIZE);
        }
    }

    int numNodes() {
        return nodeMap.size();
    }

    void toggleHidden() {
        hidden = !hidden;
    }
}
