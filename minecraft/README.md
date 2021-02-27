This is the development environment for the path finding and ARAIG simulation in
Minecraft.

### Mod
In order to run this mod, ensure that you have Fabric installed. The download can be found 
here:  https://fabricmc.net/use/

Once that's installed, you now need to install the mod that enables our mod to work. It can 
be found here: https://www.curseforge.com/minecraft/mc-mods/fabric-api

Once Fabric is installed, you can install this mod. In the mod directory, you will see a file
called `pathfindingalgorithms-minecraft-mod-X.X.X`. Copy and paste that mod into the your 
Minecraft directory. It should look something like: `AppData\Roaming\.minecraft\mods`. 

When you open Minecraft, ensure you are running it with mods. Once in, you will be able to 
run the commands that come with this mod: `/start /stop /delete /reset /hide /blind /interval <ms>`. 
These commands do exactly what's on the tin.

### Suit
In this directory, you will find the .h and .c source files which interface with 
the Java mod. Open `GraphandSuitIntegration.sln` to get started. Ensure that, 
upon opening, the .c files are contained in the `Source Files` folder and the .h files 
are open in the `Header Files` folder.

### ARAIG Integration
If you have signed the NDA and have access to the ARAIG files, copy and paste all of
the ARAIG `dll` files and the visualization tool data into the directory which you 
have created to house this mod.

To run the ARAIG visualization tool with this mod, open the tool by double clicking it.
Once it has opened, select `Live Stream` under the `Initialize` tab. Then, under `Display`,
ensure all four options are selected. It will begin streaming once an output file is 
detected.

### Development
0. Ensure you are compiling and debugging in x64.
1. Launch Minecraft with the mod.
3. Run the commands as outlined above and in-game.
2. Open the ARAIG visualization tool (if using) and run it as per the instructions above.
3. Run the debugger in Visual Studio.
4. You should now see haptic instructions on the ARAIG visualization tool and the 
reconstructed path in-game.
