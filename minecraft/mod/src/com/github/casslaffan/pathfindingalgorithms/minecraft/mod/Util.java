package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

public class Util {
    static final int DEFAULT_COLOR = 0xFF_FF_FF_FF; // RGBA White
    static final float DEFAULT_NODE_SIZE = 0.015F;

    // Szudzik's Elegant Pairing Function
    // http://szudzik.com/ElegantPairing.pdf
    static int elegantPair(int x, int y){
        return (x >= y) ? (x * x + x + y) : (y * y + x);
    }

    static float yawToAbsoluteToRad(float yaw) {
        // % 360 wraps into [-360, 360]
        // + 360 to move the range positive [0, 720]
        // % 360 again to wrap to [0, 360]
        // To radians
        return (float) Math.toRadians(((yaw % 360) + 360) % 360);
    }
}
