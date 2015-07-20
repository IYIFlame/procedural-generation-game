# procedural-generation-game
unreal 4 game with procedurally generated levels using Perlin Noise. In order to run the game simply download the repository and run pNoiseTest.exe . You move forward/backward with w/s and left and right with a and d. In order to go higher you press space and lower - ctrl. The game does not really have a goal rather than exploration, it was made as an experiment to use Perlin noise to generate space terrain. It was also a good learning opportunity to get familiar with Unreal 4 and C++.

If you wish to generate a different map you need to close the game and go inside the folder pNoiseTest and open the file map_settings.txt . Replace the specified number there with your own and then start the game again.

There is also a map image being generated at the start of the game which shows the pattern that the terrain will follow when spawning. This image can be found in /pNoiseTest/Binaries/Win64/figure_8_R.ppm . In the image the starting position of the player is in the top left corner facing downwards. 

Although the generated map is so small terrain will never stop generating around the player no matter their position because the algorithm converts the actual player coordinates to such that are inside the generated map - in other words the player position may be x but as far as the algorithm knows that position is y where x is outside the map bounds and y is inside. This is a simple and resource-cheap way of creating the illusion that the game world is endless. From this point forward only minor tweaks need to be put into place in order to make it near impossible for the user to not be able to understand that the world is actually so small.

For the Perlin noise implementation I have used as a base the implementation of user sol-prog. I have also used his library for generating ppm images. His repository can be found here https://github.com/sol-prog/Perlin_Noise .

This software is licensed under the GPL v3 license.
