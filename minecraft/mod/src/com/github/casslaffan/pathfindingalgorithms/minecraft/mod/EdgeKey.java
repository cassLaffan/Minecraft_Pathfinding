package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import java.util.Objects;

public class EdgeKey {
    private final int playerIdA;
    private final int playerIdB;
    private final int idA;
    private final int idB;

    public EdgeKey(int playerIdA, int playerIdB, int idA, int idB) {
        this.playerIdA = playerIdA;
        this.playerIdB = playerIdB;
        this.idA = idA;
        this.idB = idB;
    }

    public EdgeKey(Edge edge) {
        this.playerIdA = edge.a.playerId;
        this.playerIdB = edge.b.playerId;
        this.idA = edge.b.id;
        this.idB = edge.b.id;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        EdgeKey edgeKey = (EdgeKey) o;
        return playerIdA == edgeKey.playerIdA && playerIdB == edgeKey.playerIdB && idA == edgeKey.idA && idB == edgeKey.idB;
    }

    @Override
    public int hashCode() {
        return Objects.hash(playerIdA, playerIdB, idA, idB);
    }
}
