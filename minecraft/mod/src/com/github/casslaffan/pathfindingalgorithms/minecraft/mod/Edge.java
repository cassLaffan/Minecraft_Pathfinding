package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

public class Edge {
    Node a, b;

    // Store unpacked rgba because that's what the renderer requires. I think it's more worthwhile to store it
    // rather than unpack it every frame.
    int cr, cg, cb, ca;

    public Edge(Node a, Node b, int color) {
        this.a = a;
        this.b = b;
        update(color);
    }

    public void update(int color) {
        this.cr = (color >> 24) & 0xFF;
        this.cg = (color >> 16) & 0xFF;
        this.cb = (color >> 8) & 0xFF;
        this.ca = color & 0xFF;
    }
}
