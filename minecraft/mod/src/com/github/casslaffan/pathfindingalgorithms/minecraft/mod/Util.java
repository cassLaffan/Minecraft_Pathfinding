package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import java.awt.Color;

public class Util {
    static final int DEFAULT_COLOR = 0xFF_FF_FF_FF; // RGBA White
    static final float DEFAULT_NODE_SIZE = 0.015F;

    static long packInts(int a, int b) {
        return (((long)a) << 32) | (b & 0xffffffffL);
    }

    static float yawToAbsoluteToRad(float yaw) {
        // % 360 wraps into [-360, 360]
        // + 360 to move the range positive [0, 720]
        // % 360 again to wrap to [0, 360]
        // To radians
        return (float) Math.toRadians(((yaw % 360) + 360) % 360);
    }

    static int getPlayerNodeColor(int playerId) {
        return Color.HSBtoRGB(Math.floorMod(playerId * 1163, 360) / (float) 360, 1f, 1f) << 8;
    }
}
