package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import java.util.Objects;

public final class NodeKey {
    private final int playerId;
    private final int id;

    public NodeKey(int playerId, int id) {
        this.playerId = playerId;
        this.id = id;
    }

    public NodeKey(Node node) {
        this.playerId = node.playerId;
        this.id = node.id;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        NodeKey nodeKey = (NodeKey) o;
        return playerId == nodeKey.playerId && id == nodeKey.id;
    }

    @Override
    public int hashCode() {
        return Objects.hash(playerId, id);
    }
}
