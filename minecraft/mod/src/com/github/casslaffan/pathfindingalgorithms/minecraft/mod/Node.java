package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import net.minecraft.util.math.Vec3d;
import org.jetbrains.annotations.NotNull;

public class Node implements Comparable<Node> {
    int id;
    float x, y, z;
    int color;
    float size;

    public Node(int id, Vec3d vec) {
        this(id, (float) vec.x, (float) vec.y, (float) vec.z, Util.DEFAULT_COLOR, Util.DEFAULT_NODE_SIZE);
    }

    public Node(int id, float x, float y, float z) {
        this(id, x, y, z, Util.DEFAULT_COLOR, Util.DEFAULT_NODE_SIZE);
    }

    public Node(int id, float x, float y, float z, int color, float size) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.z = z;
        this.color = (color >>> 8); // RGBA to RGB
        this.size = size;
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
        return this.id - other.id;
    }
}
