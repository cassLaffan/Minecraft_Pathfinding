package com.github.casslaffan.pathfindingalgorithms.minecraft.mod;

import com.mojang.brigadier.CommandDispatcher;
import com.mojang.brigadier.arguments.DoubleArgumentType;
import com.mojang.brigadier.arguments.IntegerArgumentType;
import com.mojang.brigadier.arguments.StringArgumentType;
import com.mojang.brigadier.context.CommandContext;
import net.fabricmc.fabric.api.client.command.v1.FabricClientCommandSource;
import net.fabricmc.loader.api.FabricLoader;
import net.minecraft.entity.effect.StatusEffectInstance;
import net.minecraft.entity.effect.StatusEffects;

import java.util.concurrent.TimeUnit;

import static net.minecraft.text.Text.of;

import static net.fabricmc.fabric.api.client.command.v1.ClientCommandManager.literal;
import static net.fabricmc.fabric.api.client.command.v1.ClientCommandManager.argument;



public class ClientCommands {
    Mod mod = Mod.getInstance();

    void registerCommands(CommandDispatcher<FabricClientCommandSource> d) {
        d.register(literal("pf").executes(this::pf));
        d.register(literal("start").executes(this::start));
        d.register(literal("stop").executes(this::stop));
        d.register(literal("delete").executes(this::delete));
        d.register(literal("reset").executes(this::reset));
        d.register(literal("hide").executes(this::hide));
        d.register(literal("blind").executes(this::blind));
        d.register(literal("interval").then(argument("ms", IntegerArgumentType.integer(1)).executes(this::interval)));
        d.register(literal("range").then(argument("blocks", DoubleArgumentType.doubleArg(1)).executes(this::range)));
        d.register(literal("setid").then(argument("id", IntegerArgumentType.integer(1)).executes(this::setid)));
        d.register(literal("dump").then(argument("name", StringArgumentType.string()).executes(this::dump)));
    }

    private int pf(CommandContext<FabricClientCommandSource> c) {
        String help = """
            /pf - show this menu
            /start - start recording points
            /stop - stop recording points
            /delete - delete all nodes and edges
            /reset - delete all edges
            /hide - hide all nodes and edges
            /blind - toggle a blinding effect
            /interval <int:ms> - set the time between recording the next point in ms
            /range <float:blocks> - set the maximum range in blocks that you can communicate data
            /setid <int:id> - set the player id for multi-player communication""";

        c.getSource().sendFeedback(of(help));
        return 0;
    }

    private int start(CommandContext<FabricClientCommandSource> c) {
        mod.recording = true;
        return 0;
    }

    private int stop(CommandContext<FabricClientCommandSource> c) {
        mod.recording = false;
        return 0;
    }

    private int delete(CommandContext<FabricClientCommandSource> c) {
        mod.graph.delete();
        return 0;
    }

    private int reset(CommandContext<FabricClientCommandSource> c) {
        mod.graph.reset();
        return 0;
    }

    private int hide(CommandContext<FabricClientCommandSource> c) {
        mod.graph.toggleHidden();
        return 0;
    }

    private int blind(CommandContext<FabricClientCommandSource> c) {
        var p = c.getSource().getPlayer();

        if (p.hasStatusEffect(StatusEffects.BLINDNESS)) {
            p.removeStatusEffect(StatusEffects.BLINDNESS);
        } else {
            p.addStatusEffect(
                new StatusEffectInstance(StatusEffects.BLINDNESS, 300 * 20, 0, false, false)
            );
        }

        return 0;
    }

    private int interval(CommandContext<FabricClientCommandSource> c) {
        mod.intervalMs = c.getArgument("ms", int.class);
        mod.recordHandler.cancel(true);
        mod.recordHandler = mod.executor.scheduleAtFixedRate(mod::record, 0, mod.intervalMs, TimeUnit.MILLISECONDS);
        return 0;
    }

    private int range(CommandContext<FabricClientCommandSource> c) {
        mod.radio.range = c.getArgument("blocks", double.class);;
        return 0;
    }

    private int setid(CommandContext<FabricClientCommandSource> c) {
        int pId = c.getArgument("id", int.class);
        int seqId = 1;

        for (Node n : mod.graph.nodeMap.values()) {
            if (n.playerId == pId && n.sequenceId > seqId) {
                seqId = n.sequenceId;
            }
        }

        Node.clientPlayerId = pId;
        Node.clientSequenceId = seqId;
        return 0;
    }

    private int dump(CommandContext<FabricClientCommandSource> c) {
        c.getSource().sendFeedback(of(FabricLoader.getInstance().getConfigDir().toString()));
        c.getSource().sendFeedback(of(FabricLoader.getInstance().getGameDir().toString()));
        return 0;
    }
}
