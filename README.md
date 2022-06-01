# Minecraft Pathfinding
These are the algorithms and Minecraft mod which accompany this paper:

https://ieeexplore.ieee.org/document/9631428

And another paper currently in the works.

### Mod
In order to run this mod, ensure that you have Fabric installed. The download can be found 
here:  https://fabricmc.net/use/

Once that's installed, you now need to install the mod that enables our mod to work. It can 
be found here: https://www.curseforge.com/minecraft/mc-mods/fabric-api

Once both of the above are installed, you can install this mod. In the mod directory, you will see a file
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

### Development
0. Ensure you are compiling and debugging in x64.
1. Launch Minecraft with the mod.
2. Run the commands as outlined above and in-game.
3. Run the debugger in Visual Studio.
4. You should now see the reconstructed path and directions in-game.

### Note
If you wish to have access to the ARAIG functionality as described in the accompanying paper for this repo,
you'll need to reach out to IFTech and request access to the ARAIG SDK. The form to request acces can 
be found here: https://iftech-technologies.com/download-the-sdk/
