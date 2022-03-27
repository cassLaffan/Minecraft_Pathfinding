package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import net.minecraft.util.math.Vec3d;
import org.jetbrains.annotations.NotNull;

import java.util.Random;

public class Node implements Comparable<Node> {
    private static final Random random = new Random();
    public static int clientSequenceId = 0;
    public static int clientPlayerId = random.nextInt() >>> 17;

    int id;
    int playerId;
    int sequenceId;
    float x, y, z;
    int color;
    float size;

    public Node(Vec3d vec) {
        this(newId(), (float) vec.x, (float) vec.y, (float) vec.z, Util.DEFAULT_NODE_SIZE);
    }

    public Node(int id, float x, float y, float z) {
        this(id, x, y, z, Util.DEFAULT_NODE_SIZE);
    }

    public Node(int id, float x, float y, float z, float size) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.z = z;
        this.size = size;
        this.playerId = id >>> 16;
        this.sequenceId = 0xFFFF & id;
        this.color = Util.getPlayerNodeColor(this.playerId) >>> 8;
    }

    public void update(float x, float y, float z, int color, float size) {
        if (y > 0) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        if (size > 0) {
            this.size = size;
        }

        this.color = (color >>> 8); // RGBA to RGB
    }

    @Override
    public int compareTo(@NotNull Node other) {
        if (playerId == other.playerId) {
            return sequenceId - other.sequenceId;
        }

        return playerId - other.playerId;
    }

    public static int newId() {
        return (clientPlayerId << 16) | (clientSequenceId++);
    }
}
